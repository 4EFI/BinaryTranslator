
Стоит сразу оговориться, что работа будет состоять из двух частей. В первой части мы напишем JIT (Just-In-Time) компилятор, а во второй - полноценный бинарный транслятор (AOT - Ahead-Of-Time компилятор) с записью в ELF (Executable and Linkable Format) файл.

# Часть 1. JIT-компилятор

## Введение

В прошлом семестре мы написали свой язык программирования ([triple 'p'](https://github.com/4EFI/Language)). Язык, который плюсовее плюсов, ведь это плюс плюс плюс :)) Написанный язык выполнялся на [процессоре](https://github.com/4EFI/CPU) реализованном нами. В данной работе нам предстоит транслировать наш байт код в нативный код архитектуры x86-64.

<p style="text-align: center"><img src=res/bin.png width="750px"/></p>

## Реализация трансляции

### Начало начал. Virtual CPU 

Сперва, давайте углубимся в структуру команд нашего процессора. Первый байт, а именно первые 5 бит отвечают за "cmd num", за номер соответствующей команды (0-31), далее следует бит "imm", бит который показывает, оперирует ли команда со значениями, следующий бит - "reg" - будет ли присутствовать регистр, последний бит "mem" показывает, будут ли производится действия с оперативной памяти. Изобразим это схематично: 

<p style="text-align: center"><img src=res/cmd.png width="500px"/></p>

После байта команды, следует байт "reg num", отвечающий за номер регистра, если бит "reg" выставлен в 1. Затем следуют 8 байт, значение типа double, если бит "imm" выставлен в 1.

<p style="text-align: center"><img src=res/full_cmd.png width="550px"/></p>

Теперь приведу пример листинга моего байт кода, для лучшего представления структуры команд:

<p style="text-align: center"><img src=res/example_asm.png width="600px"/></p>

После того, как мы имеем представление о том, как ассемблерный код переводится в машинный, давайте прочтем наш бинарник из файла в память, предварительно сравнив сигнатуру и версию. Затем разобьем его на массив структур следующего вида:

```C++
struct Command
{
    CMD cmd;

    int    reg_num;
    double val;
};
```

Это сильно упрощает процесс обработки команд, также нам не нужно будет каждый раз обращаться к файлу, что очень затратно, мы один раз считаем его полностью и забудем. Таким образом мы реализовали [IR](https://www.mono-project.com/docs/advanced/runtime/docs/linear-ir/) (кхм..кхм.. интермидиат репрезентейшен). Это не закон Ома блин. 

Перед тем как начать транслировать в машинный код архитектуры x86-64, стоит отметить, что единственным типом моего процессора является тип double, поэтому для того, чтобы производить арифметические операции над ними в архитектуре x86-64, будем использовать xmm регистры и соответствующие ассемблерные команды. 

Перейдем к следующему этапу, непосредственно к трансляции.

### Трансляция Virtual CPU ➜ x86-64

#### Push/Pop. Или как Владос со стеком баловался

Я бы сказал, это одна из самых сложных частей, ведь нам предстоит решить сразу несколько задач. Начнем с простого, трансляции команды байт кода ```push r_x```, где r_x это один из регистров виртуального процессора (rax, rcx, rdx, rbx). 

| байт код     | нативный код x86-64     |
|--------------|-------------------------|
| push r_x     | push r_x                |

> Стоит отметить, что трансляция производится на бинарном уровне, и лишь для простоты, я описываю трансляцию на ассемблере. 

В моем процессоре была реализована возможность пушить числа в сумме с регистром, разберем этой случай:

| байт код       | нативный код x86-64     |
|----------------|-------------------------|
| push r_x + val | mov r10, val <br/> movq xmm1, r10 <br/> movq xmm0, r_x <br/> addsd xmm0, xmm1 <br/> movq r10, xmm0 <br/> push r10 |

А что делать если мы встречаем команду ```push [ ]```. Во первых нам нужна заранее созданная оперативная память, как мы это делали в нашем процессоре. Поэтому перед началом обработки команд, выделим ее. Трансляция почти не отличается от пуша регистра плюс число:

| байт код       | нативный код x86-64     |
|----------------|-------------------------|
| push r_x + val | mov r10, ram_addr <br/> --//-- <br/> cvttsd2si r10, xmm0 <br/> push qword [r10] |


Первой строчкой прибавляем адрес начала оперативной памяти. Мы должны конвертировать сумму регистра и числа из double в int при помощи ```cvttsd2si```.


После того как мы реализовали ```push```, разрешили множество проблем, которые встретились нам на пути, с ```pop``` проблем не возникло и сделали все по аналогии. 

> Рубрика "не баг а фича". Так как процессор написан на даблах, я оставил возможность ```push [rax + 0.5]```, что именно он запушет, я предоставляю возможность разобраться заинтересованному читателю в качестве упражнения. 

#### Арифметика. История о том, как Перец считать учился

В моем процессоре была реализована возможность умножения (mul), деления (div), сложения (add) и вычитания (sub). Рассмотрим арифметику на примере сложения двух чисел, вся остальная арифметика пишется аналогично:


| байт код       | нативный код x86-64     |
|----------------|-------------------------|
| push val1 <br/> push val2 <br/> add | push val1 <br/> push val2 <br/> movsd xmm1, [rsp] <br/> add rsp, 8 <br/> mov xmm0, [rsp] <br/> addsd xmm0, xmm1 <br/> movsd [rsp], xmm0 |

Так как push мы уже реализовали, то арифметические операции не вызывают трудностей в написании.

#### Условные и безусловные джампы. Джамп джамп

Так как адресация в моем процессоре и x86-64 отличается, нам нужно придумать как решить эту проблему. Я к этому подошел следующим образом, сначала я добавил в структуру положения команды относительно моего байт кода и относительно нативного кода архитектуры x86-64:

```C++
struct Command
{
    ...

    int   bin_code_pos;
    char* bin_code_x86_ptr;
};
```

Очень удобно, что мы реализовали IR и он позволяет с такой легкостью добавлять данные. 

Перед началом обработки команд, мы создаем таблицу джампов. Как только мы встречаем ```jmp```, мы транслируем это как ```jmp ...``` и добавляем в таблицу указатель на начала пропуска и указатель на позицию в которую будет производиться джамп. По завершению обработки всех команд, мы проходимся по этой таблице и заполняем пропуски. 

Для того, чтобы реализовать условные переходы, нам предстоит загрузить в xmm0 и xmm1 регистры значения из стека, сравнить их с помощью **cmpsd xmm0, xmm1, TYPE** и в зависимости от результата выполнить переход или нет.

| байт код       | нативный код x86-64     |
|----------------|-------------------------|
| push val1 <br/> push val2 <br/> je my_addr | push val1 <br/> push val2 <br/> movsd xmm1, [rsp] <br/> add rsp, 8 <br/> mov xmm0, [rsp] <br/> add rsp, 8 <br/> cmpeqsd xmm0, xmm1 <br/> movq r10, xmm0 <br/> cmp r10, 0 <br/> jne ... |

#### Call/Ret. Меня вызывали?

При трансляции вызова функции, я столкнулся с проблемой, что в моем процессоре было 2 стека, один из них использовался для значений, второй для адресов возврата. В этом случае, если будем использовать лишь один системный стек в x86-64, то при стековой передаче параметров в функцию, будет браться адрес возврата вместо нужных нам переменных. Поэтому я выдел память и использую ее как стек адресов возврата. В нативном коде, я повесил данный стек на регистр ```rbp```. Адресы возврата пушу именно туда. Все остальное выполняется по аналогии ```jmp```.

| байт код     | нативный код x86-64     |
|--------------|-------------------------|
| call func    | add rbp, 8 <br/> mov r10, curr_pos <br/> mov [rbp], r10 <br/> jmp ... |

Ret транслируется следующим образом, выпопливается значение со стека возвратов в системный стек и выполняется ret. 

| байт код     | нативный код x86-64     |
|--------------|-------------------------|
| ret          | push qword [rbp] <br/> sub rbp, 8 <br/> ret |

#### Вывод. Да не тот вывод, а в консоль

Для вывода я решил осмелиться и использовать свой ранее написанный [```printf```](https://github.com/4EFI/Printf). Правда он работает на целых числах, но ничего, просто конвертируем из double в int и проблем нет, ну точность неем-м-н-о-о-о-о-го теряется, подумаешь.  

# Часть 2. AOT-компилятор

В данной части работы, мы продолжим наш JIT-компилятор. Мой байт код, транслированный в нативный код архитектуры x86-64, запишем в определенном формате, а именно ELF (Executable and Linkable Format). Для того чтобы хоть что-то начало получаться, пришлось посетить не мало статей и форумов, а проблем и ошибок было не меньше. Сейчас я постараюсь ввести вас в структуру исполняемого файла. 

В Си есть прекрасная библиотека **<elf.h>**, которая упрощает нашу работу. Также в процессе написания очень помогла утилита **pax-utils/dumpelf**, позволяющая считывать все внутренние структуры ELF в эквивалентные структуры Си для отладки. Первым делом, что пришлось сделать, это записать в файл его заголовок: 

```C++
Elf64_Ehdr elf_header = 
    {
        .e_ident = 
        { 
            /* (EI_NIDENT bytes)     */ 
            /* [0] EI_MAG:           */ ELFMAG0      , /* 0x7F */
                                        ELFMAG1      , /* 'E'  */
                                        ELFMAG2      , /* 'L'  */
                                        ELFMAG3      , /* 'F'  */
            /* [4] EI_CLASS:         */ ELFCLASS64   , /*  2   */
            /* [5] EI_DATA:          */ ELFDATA2LSB  , /*  1   */
            /* [6] EI_VERSION:       */ EV_CURRENT   , /*  1   */
            /* [7] EI_OSABI:         */ ELFOSABI_NONE, /*  0   */
            /* [8] EI_ABIVERSION:    */ 0,
            /* [9-15] EI_PAD:        */ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
        },
        .e_type      = ET_EXEC    , /* 2  */
        .e_machine   = EM_X86_64  , /* 62 */
        .e_version   = EV_CURRENT , /* 1  */
        .e_entry     = TEXT_ADDR  , /* (start address at runtime) */
        .e_phoff     = 64         , /* (bytes into file) */
        .e_shoff     = ELF_SIZE   , /* (bytes into file) */
        .e_flags     = 0x0        ,
        .e_ehsize    = 64         , /* (bytes) */
        .e_phentsize = 56         , /* (bytes) */
        .e_phnum     = 5          , /* (program headers) */
        .e_shentsize = 64         , /* (bytes) */
        .e_shnum     = 0          , /* (section headers) */
        .e_shstrndx  = 0         
    };
```

Теперь мы должны описать таблицу заголовков программы. У меня их будет 5. Заголовки описываются следующем образом:

```C++
.phdrs = {
/* Program Header #0 0x40 */
{
	.p_type   = 1          , /* [PT_LOAD] */
	.p_offset = 0          , /* (bytes into file) */
	.p_vaddr  = 0x400000   , /* (virtual addr at runtime) */
	.p_paddr  = 0x400000   , /* (physical addr at runtime) */
	.p_filesz = 176        , /* (bytes in file) */
	.p_memsz  = 176        , /* (bytes in mem at runtime) */
	.p_flags  = 0x4        , /* PF_R */
	.p_align  = 4096       , /* (min mem alignment in bytes) */
};
```

Помимо нативного кода архитектуры x86-64, мне пришлось описать блок выделяемый под оперативную память, а также память под стек. После этого, пришлось немного поменять адреса в нативном коде, так как при JIT-компиляции, оперативная память и стек адресов возврата располагались на других местах. 

Самое интересное, это добавление библиотеки. Так как при JIT-компиляции я использую свой ранее написанный printf, его нужно расположить в ELF файле. Для начала, я скомпилировал свою библиотеку в [bin](https://www.opennet.ru/docs/RUS/nasm/nasm_ru6.html) формате, предварительно директивой org задавав начальный адрес программы, с которого она располагается при загрузке в память, это нужно для того, чтобы правильно работали все переходы, а также вызовы функций. Затем добавляем в таблицу заголовков описание блока кода с ```printf```, и по указанному смещению записываем скомпилированный нами bin файл. Затем опять в нативном коде изменяем адрес моей функции ```printf```.

> Линковка с объектным файлом показалась мне куда сложнее, поэтому я решил реализовать это именно так.

## Ускорение

Давайте измерим время выполнения одинаковых программ, а именно решения квадратного уравнения вместе с подсчетом факториала числа, на виртуальном процессоре и процессоре архитектуры x86-64. Но слабых бить нельзя, поэтому будем сражаться с ```-O3```:

|    процессор            | наносекунд                    | ускорение  |
|-------------------------|-------------------------------|------------|
| виртуальный с ```-O3``` |            196 781            |  1.00      |
| X86-64                  |             17 761            |  11.08     |

## Заключение

В заключении хочу привести небольшой пример трансляции моего байт кода (слева), в нативный код архитектуры x86-64 (справа):

<p style="text-align: center"><img src=res/final.png width="100%"/></p>

Как вы можете заметить, в трансляции присутствует большое количество ```nop```, это нужно для разделения команд между собой и простоты отладки.  

Стоит отметить, что даже сильного противника ```-O3``` мы смогли одолеть и обойти виртуальный процессор в 11 раз.

Конечно, я описал лишь основную логику трансляции команд, более подробнее вы можешь ознакомиться, покопавшись непосредственно в [коде](https://github.com/4EFI/BinaryTranslator/blob/main/LngHandler/execution/include/commands.h). Как только у меня получилось транслировать свой байт код, я почувствовал себя богом Си и асма, желаю прочувствовать это всем на себе :))  


