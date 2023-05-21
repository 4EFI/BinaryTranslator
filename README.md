
Стоит сразу оговориться, что работа будет состоять из двух частей. В первой части мы напишем JIT компилятор, а во второй - полноценный бинарный транслятор с записью в ELF (Executable and Linkable Format) файл.

# Часть 1. JIT-компилятор

## Введение

В прошлом семестре мы написали свой язык программирования ([triple 'p'](https://github.com/4EFI/Language)). Язык, который плюсовее плюсов, ведь это плюс плюс плюс :)) Написанный язык выполнялся на [процессоре](https://github.com/4EFI/CPU) реализованном нами. В данной работе нам предстоит транслировать наш байт код в нативный код архитектуры x86-64.

<p style="text-align: center"><img src=res/bin.png width="750px"/></p>

## Реализация трансляции

### Начало начал. База базная 

Сперва, давайте углубимся в структуру команд нашего процессора. Первый байт, а именно первые 5 бит отвечают за "cmd num", за номер соответствующей команды (0-31), далее следует бит "imm", бит который показывает, оперирует ли команда со значениями, следующий бит - "reg" - будет ли присутствовать регистр, последний бит "mem" показывает, будут ли производится действия с оперативной памяти. Изобразим это схематично: 

<p style="text-align: center"><img src=res/cmd.png width="500px"/></p>

После байта команды, следует байт "reg num", отвечающий за номер регистра, если бит "reg" выставлен в 1. Затем следуют 8 байт, значение типа double, если бит "imm" выставлен в 1.

<p style="text-align: center"><img src=res/full_cmd.png width="600px"/></p>

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

### Push/Pop. Или как Владос со стеком баловался

Я бы сказал, это одна из самых сложных частей, ведь нам предстоит решить сразу несколько задач. Начнем с простого, трансляции команды байт кода **push r_x**, где r_x это один из регистров виртуального процессора (rax, rcx, rdx, rbx). 

| байт код     | нативный код x86-64     |
|--------------|-------------------------|
| push r_x     | push r_x                |

> Стоит отметить, что трансляция производится на бинарном уровне, и лишь для простоты, я описываю трансляцию на ассемблере. 

В моем процессоре была реализована возможность пушить числа в сумме с регистром, разберем этой случай:

| байт код       | нативный код x86-64     |
|----------------|-------------------------|
| push r_x + val | mov r10, val <br/> movq xmm1, r10 <br/> movq xmm0, r_x <br/> addsd xmm0, xmm1 <br/> movq r10, xmm0 <br/> push r10 |

А что делать если мы встречаем команду **push [ ]**. Во первых нам нужна заранее созданная оперативная память, как мы это делали в нашем процессоре. Поэтому перед началом обработки команд, выделим ее. Трансляция почти не отличается от пуша регистра плюс число:

| байт код       | нативный код x86-64     |
|----------------|-------------------------|
| push r_x + val | mov r10, ram_addr <br/> --//-- <br/> cvttsd2si r10, xmm0 <br/> push qword [r10] |


Первой строчкой прибавляем адрес начала оперативной памяти. Мы должны конвертировать сумму регистра и числа из double в int при помощи **cvttsd2si**.


После того как мы реализовали **push**, разрешили множество проблем, которые встретились нам на пути, с **pop** проблем не возникло и сделали все по аналогии. 

> Рубрика "не баг а фича". Так как процессор написан на даблах, я оставил возможность push [rax + 0.5], что именно он запушет, я предоставляю возможность разобраться заинтересованному читателю в качестве упражнения. 

### Арифметика. История о том, как Перец считать учился

В моем процессоре была реализована возможность умножения(mul), деления(div), сложения(add) и вычитания(sub). Рассмотрим арифметику на примере сложения двух чисел, вся остальная арифметика пишется аналогично:


| байт код       | нативный код x86-64     |
|----------------|-------------------------|
| push val1 <br/> push val2 <br/> add | push val1 <br/> push val2 <br/> movsd xmm1, [rsp] <br/> add rsp, 8 <br/> mov xmm0, [rsp] <br/> addsd xmm0, xmm1 <br/> movsd [rsp], xmm0 |

Так как push мы уже реализовали, то арифметические операции не вызывают трудностей в написании.

### Условные и безусловные джампы. Джамп джамп

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

Перед началом обработки команд, мы создаем таблицу джампов. Как только мы встречаем **jmp**, мы транслируем это как jmp ... и добавляем в таблицу указатель на начала пропуска и указатель на позицию в которую будет производиться джамп. По завершению обработки всех команд, мы проходимся по этой таблице и заполняем пропуски. 

Для того, чтобы реализовать условные переходы, нам предстоит загрузить в xmm0 и xmm1 регистры значения из стека, сравнить их с помощью **cmpsd xmm0, xmm1, TYPE** и в зависимости от результата выполнить переход или нет.

| байт код       | нативный код x86-64     |
|----------------|-------------------------|
| push val1 <br/> push val2 <br/> je my_addr | push val1 <br/> push val2 <br/> movsd xmm1, [rsp] <br/> add rsp, 8 <br/> mov xmm0, [rsp] <br/> add rsp, 8 <br/> cmpeqsd xmm0, xmm1 <br/> movq r10, xmm0 <br/> cmp r10, 0 <br/> jne ... |

### Call/Ret. Меня вызывали?

При трансляции вызова функции, я столкнулся с проблемой, что в моем процессоре было 2 стека, один из них использовался для значений, второй для адресов возврата. В этом случае, если будем использовать лишь один системный стек в x86-64, то при стековой передаче параметров в функцию, будет браться адрес возврата вместо нужных нам переменных. Поэтому я выдел память и использую ее как стек адресов возврата. В нативном коде, я повесил данный стек на регистр **rbp**. Адресы возврата пушу именно туда. Все остальное выполняется по аналогии jmp.

| байт код     | нативный код x86-64     |
|--------------|-------------------------|
| call func    | add rbp, 8 <br/> mov r10, curr_pos <br/> mov [rbp], r10 <br/> jmp ... |

Ret транслируется следующим образом, выпопливается значение со стека возвратов в системный стек и выполняется ret. 

| байт код     | нативный код x86-64     |
|--------------|-------------------------|
| ret          | push qword [rbp] <br/> sub rbp, 8 <br/> ret |

### Вывод. Да не тот вывод, а в консоль

Для вывода я решил осмелиться и использовать свой ранее написанный [printf](https://github.com/4EFI/Printf). Правда он работает на целых числах, но ничего, просто конвертируем из double в int и проблем нет, ну точность неем-м-н-о-о-о-о-го теряется, подумаешь.  

## Замеры

Давайте измерим время выполнения одинаковых программ на виртуальном процессоре и процессоре архитектуры x86-64. Но слабых бить нельзя, поэтому будем сражаться с -O3:

|    процессор        | наносекунд                    | ускорение  |
|---------------------|-------------------------------|------------|
| виртуальный с "-O3" |            196 781            |  1.00      |
| X86-64              |             17 761            |  11.08     |

Как мы можем видеть, даже сильного противника "-O3" мы смогли одолеть и обойти виртуальный процессор в 11 раз.

# Часть 2. Бинарный транслятор

## Заключение

В заключении хочу привести небольшой пример трансляции моего байт кода(слева), в нативный код архитектуры x86-64(справа):

<p style="text-align: center"><img src=res/final.png width="750px"/></p>

Как вы можете заметить, в трансляции присутствует большое количество nop, это нужно для разделения команд между собой и простоты отладки.  

Конечно, я описал лишь основную логику трансляции команд, более подробнее вы можешь ознакомиться, покопавшись непосредственно в [коде](https://github.com/4EFI/BinaryTranslator/blob/main/LngHandler/execution/include/commands.h). Как только у меня получилось транслировать свой байт код, я почувствовал себя богом Си и асма, желаю прочувствовать это всем на себе :))  


