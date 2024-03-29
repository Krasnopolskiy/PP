# НИЯУ МИФИ. Лабораторная работа №4. Краснопольский Иван, Б21-525. 2023

## Используемая система

### Используемая система:

```text
System:
  Kernel: 6.2.0-20-generic arch: x86_64 bits: 64 compiler: gcc v: 12.2.0
    Distro: Ubuntu 23.04 (Lunar Lobster)
CPU:
  Info: 32x 1-core model: Intel Xeon (Icelake) bits: 64 type: SMP smt: <unsupported>
    arch: Tremont Snow Ridge rev: 0 cache: L1: 32x 64 KiB (2 MiB) L2: 32x 4 MiB (128 MiB)
    L3: 32x 16 MiB (512 MiB)
  Speed (MHz): avg: 2195 min/max: N/A cores: 1: 2195 2: 2195 3: 2195 4: 2195 5: 2195 6: 2195
    7: 2195 8: 2195 9: 2195 10: 2195 11: 2195 12: 2195 13: 2195 14: 2195 15: 2195 16: 2195 17: 2195
    18: 2195 19: 2195 20: 2195 21: 2195 22: 2195 23: 2195 24: 2195 25: 2195 26: 2195 27: 2195
    28: 2195 29: 2195 30: 2195 31: 2195 32: 2195 bogomips: 140469
```

### Среда разработки

- Язык программирования C
- Версия gcc: `11.4.0`
- Версия OpenMP: `Release: 201511 OpenMP: 4.5`
- Версия OpenMPI: `(OpenRTE) 4.1.4`

## Пример вычислительного алгоритма, использующего механизм явных блокировок

Алгоритм выполняет параллельное сложение элементов массива.

Блокировка гарантирует, что вся операция сложения `sum += array[i]` будет выполнена атомарно. Без блокировки, возможна
ситуация, когда один поток начинает операцию обновления, но до её завершения другой поток начинает своё обновление, в
результате чего часть данных теряется или искажается. Благодаря блокировке, итоговое значение sum будет отражать полную
сумму всех элементов массива array, поскольку каждое обновление будет выполнено полностью и последовательно.

## Анализ OpenMP директив

### Анализ окружения

- `_OPENMP`: Эта макроконстанта определяет версию OpenMP.
- `omp_get_num_procs()`: Возвращает количество процессоров доступных для программы.
- `omp_get_max_threads()`: Возвращает максимально возможное количество потоков.
- `omp_get_dynamic()`: Показывает, включена ли динамическая регулировка числа потоков.
- `omp_get_wtick()`: Возвращает точность таймера.
- `omp_get_nested()`: Показывает, включена ли вложенная параллельность.
- `omp_get_max_active_levels()`: Возвращает максимальное количество активных уровней параллельности.
- `omp_get_schedule()`: Возвращает текущую политику планирования и размер чанка.

### Алгоритм, использующий механизм явных блокировок

- `omp_lock_t`: Тип данных для блокировок.
- `omp_init_lock()`: Инициализирует блокировку.
- `#pragma omp parallel for`: Директива для выполнения цикла for в параллельном режиме.
- `omp_set_lock(), omp_unset_lock()`: Устанавливает и снимает блокировку соответственно.
- `omp_destroy_lock()`: Уничтожает объект блокировки.

### Программа для исследования режимов распределения 

- `omp_get_wtime()`: Возвращает текущее время (используется для измерения времени выполнения).
- `#pragma omp parallel num_threads(threads)`: Запускает параллельный блок с указанным количеством потоков.
- `shared`: Ключевое слово, определяющее общие переменные для всех потоков.
- `reduction(max`: max)`: Директива для выполнения операции редукции (в данном случае, поиска максимального значения).
- `default(none)`: Указывает, что каждая переменная должна быть явно определена.
- `#pragma omp for schedule(static | dynamic | guided, chunk)`:
    - Распределение итераций выполняется заранее в равных частях между потоками. Если размер чанка не указан, итерации
      делятся поровну. Если размер чанка указан, каждый поток выполняет группы итераций фиксированного размера.
    - В этом режиме потоки запрашивают новый чанк итераций после завершения предыдущего. Это полезно, когда время
      выполнения итераций значительно варьируется, так как помогает балансировать нагрузку между потоками. Размер чанка
      может быть указан, чтобы контролировать количество итераций, обрабатываемых за один раз.
    - Похоже на динамическое распределение, но размер чанка уменьшается со временем, оптимизируя баланс между
      управлением нагрузкой и накладными расходами на синхронизацию.

## Вычисления

### Анализ окружения

```txt
OpenMP Version 201511
Number of processors: 32
Maximum number of threads: 32
Dynamic threads are disabled
Timer resolution: 1e-09 seconds
Nested parallelism is disabled
Max active levels: 1
Schedule: 2, chunk size: 1
```

### Тип разделения: auto

- <details>
  <summary>Размер фрагмента: auto</summary>

  ```text
  Threads	Time
  1	0.030894
  2	0.015713
  3	0.010437
  4	0.008325
  5	0.006337
  6	0.005347
  7	0.005204
  8	0.004569
  9	0.003781
  10	0.003587
  11	0.003097
  12	0.002961
  13	0.002867
  14	0.002748
  15	0.002467
  16	0.002368
  17	0.002306
  18	0.002100
  19	0.002097
  20	0.002015
  21	0.001899
  22	0.001908
  23	0.001780
  24	0.001746
  25	0.001654
  26	0.001618
  27	0.001566
  28	0.001518
  29	0.001501
  30	0.001457
  31	0.001409
  32	0.001362
  33	0.002303
  34	0.002372
  35	0.002396
  36	0.002487
  37	0.002395
  38	0.002439
  39	0.002390
  40	0.002384
  41	0.002375
  42	0.002344
  43	0.002260
  44	0.002324
  45	0.002420
  46	0.002318
  47	0.002392
  48	0.002323
  49	0.002340
  50	0.002430
  51	0.002476
  52	0.002411
  53	0.002349
  54	0.002461
  55	0.002438
  56	0.002366
  57	0.002457
  58	0.002300
  59	0.002407
  60	0.002248
  61	0.002318
  62	0.002370
  63	0.002381
  64	0.002364
  ```

</details>

### Тип разделения: static

- <details>
  <summary>Размер фрагмента: 100</summary>

  ```text
  Threads	Time
  1	0.028263
  2	0.014532
  3	0.010261
  4	0.008086
  5	0.007228
  6	0.006458
  7	0.005290
  8	0.004783
  9	0.003947
  10	0.003714
  11	0.003314
  12	0.003383
  13	0.003189
  14	0.003097
  15	0.002796
  16	0.002645
  17	0.002627
  18	0.002566
  19	0.002349
  20	0.002251
  21	0.002190
  22	0.002048
  23	0.002002
  24	0.002104
  25	0.002017
  26	0.001841
  27	0.001780
  28	0.001749
  29	0.001699
  30	0.001742
  31	0.001616
  32	0.001596
  33	0.002632
  34	0.002640
  35	0.002641
  36	0.002626
  37	0.002649
  38	0.002542
  39	0.002666
  40	0.002606
  41	0.002662
  42	0.002519
  43	0.002590
  44	0.002665
  45	0.002582
  46	0.002562
  47	0.002727
  48	0.002587
  49	0.002701
  50	0.002539
  51	0.002616
  52	0.002692
  53	0.002553
  54	0.002683
  55	0.002680
  56	0.002672
  57	0.002753
  58	0.002646
  59	0.002781
  60	0.002605
  61	0.002658
  62	0.002453
  63	0.002537
  64	0.002704
  ```

</details>

- <details>
  <summary>Размер фрагмента: 10000</summary>

  ```text
  Threads	Time
  1	0.030941
  2	0.015517
  3	0.010451
  4	0.008061
  5	0.006309
  6	0.005694
  7	0.004585
  8	0.004403
  9	0.004207
  10	0.003612
  11	0.003656
  12	0.003042
  13	0.002649
  14	0.002636
  15	0.002365
  16	0.002787
  17	0.002154
  18	0.002210
  19	0.002105
  20	0.002013
  21	0.001895
  22	0.001878
  23	0.001793
  24	0.001734
  25	0.001690
  26	0.001718
  27	0.001639
  28	0.001541
  29	0.001509
  30	0.001469
  31	0.001428
  32	0.001400
  33	0.002248
  34	0.002264
  35	0.002315
  36	0.002355
  37	0.002346
  38	0.002325
  39	0.002297
  40	0.002326
  41	0.002320
  42	0.002258
  43	0.002303
  44	0.002280
  45	0.002287
  46	0.002238
  47	0.002928
  48	0.002834
  49	0.002668
  50	0.002745
  51	0.002539
  52	0.002682
  53	0.002555
  54	0.002462
  55	0.002718
  56	0.002467
  57	0.002642
  58	0.002548
  59	0.002590
  60	0.002600
  61	0.002624
  62	0.002542
  63	0.002430
  64	0.002897
  ```

</details>

### Тип разделения: dynamic

- <details>
  <summary>Размер фрагмента: 100</summary>

  ```text
  Threads	Time
  1	0.027873
  2	0.018349
  3	0.012723
  4	0.010018
  5	0.008398
  6	0.007310
  7	0.006334
  8	0.005811
  9	0.005382
  10	0.005249
  11	0.004910
  12	0.004601
  13	0.004657
  14	0.004815
  15	0.004789
  16	0.004528
  17	0.004804
  18	0.004648
  19	0.004458
  20	0.004539
  21	0.004359
  22	0.004419
  23	0.004435
  24	0.004494
  25	0.004410
  26	0.004436
  27	0.004512
  28	0.004236
  29	0.004331
  30	0.004077
  31	0.004007
  32	0.003926
  33	0.004093
  34	0.004145
  35	0.004282
  36	0.004391
  37	0.004171
  38	0.004213
  39	0.004120
  40	0.004211
  41	0.004319
  42	0.004166
  43	0.004370
  44	0.004315
  45	0.004100
  46	0.004123
  47	0.004232
  48	0.004170
  49	0.004186
  50	0.004404
  51	0.004425
  52	0.004434
  53	0.004306
  54	0.004470
  55	0.004360
  56	0.004388
  57	0.004449
  58	0.004277
  59	0.004470
  60	0.004415
  61	0.004362
  62	0.004283
  63	0.004289
  64	0.004277
  ```

</details>

- <details>
  <summary>Размер фрагмента: 10000</summary>

  ```text
  Threads	Time
  1	0.030430
  2	0.015273
  3	0.010232
  4	0.007691
  5	0.006184
  6	0.005321
  7	0.004448
  8	0.003931
  9	0.003494
  10	0.003151
  11	0.002946
  12	0.002742
  13	0.002526
  14	0.002381
  15	0.002203
  16	0.002043
  17	0.001973
  18	0.001863
  19	0.001757
  20	0.001703
  21	0.001655
  22	0.001582
  23	0.001553
  24	0.001467
  25	0.001394
  26	0.001371
  27	0.001341
  28	0.001290
  29	0.001260
  30	0.001240
  31	0.001205
  32	0.001188
  33	0.001403
  34	0.001400
  35	0.001413
  36	0.001509
  37	0.001427
  38	0.001407
  39	0.001456
  40	0.001449
  41	0.001495
  42	0.001446
  43	0.001474
  44	0.001510
  45	0.001476
  46	0.001482
  47	0.001532
  48	0.001505
  49	0.001522
  50	0.001509
  51	0.001563
  52	0.001514
  53	0.001537
  54	0.001577
  55	0.001600
  56	0.001607
  57	0.001620
  58	0.001599
  59	0.001588
  60	0.001579
  61	0.001626
  62	0.001576
  63	0.001585
  64	0.001598
  ```

</details>

### Тип разделения: guided

- <details>
  <summary>Размер фрагмента: 100</summary>

  ```text
  Threads	Time
  1	0.030198
  2	0.015149
  3	0.011378
  4	0.007680
  5	0.006137
  6	0.005111
  7	0.004396
  8	0.003918
  9	0.003456
  10	0.003229
  11	0.002928
  12	0.002661
  13	0.002436
  14	0.002302
  15	0.002143
  16	0.002004
  17	0.001988
  18	0.001864
  19	0.001789
  20	0.001701
  21	0.001695
  22	0.001566
  23	0.001531
  24	0.001501
  25	0.001438
  26	0.001385
  27	0.001323
  28	0.001304
  29	0.001251
  30	0.001219
  31	0.001228
  32	0.001181
  33	0.001487
  34	0.001543
  35	0.001529
  36	0.001655
  37	0.001644
  38	0.001560
  39	0.001683
  40	0.001743
  41	0.001784
  42	0.001759
  43	0.001806
  44	0.001792
  45	0.001896
  46	0.001887
  47	0.001818
  48	0.001932
  49	0.001837
  50	0.001850
  51	0.001931
  52	0.001890
  53	0.001936
  54	0.001962
  55	0.001947
  56	0.001937
  57	0.001935
  58	0.001958
  59	0.002004
  60	0.001960
  61	0.001967
  62	0.001931
  63	0.001943
  64	0.001905
  ```

</details>

- <details>
  <summary>Размер фрагмента: 10000</summary>

  ```text
  Threads	Time
  1	0.030072
  2	0.015118
  3	0.010122
  4	0.007623
  5	0.006142
  6	0.005145
  7	0.004414
  8	0.003888
  9	0.003477
  10	0.003123
  11	0.002854
  12	0.002745
  13	0.002470
  14	0.002300
  15	0.002177
  16	0.002019
  17	0.001931
  18	0.001868
  19	0.001755
  20	0.001669
  21	0.001621
  22	0.001573
  23	0.001520
  24	0.001440
  25	0.001390
  26	0.001350
  27	0.001298
  28	0.001276
  29	0.001286
  30	0.001261
  31	0.001211
  32	0.001180
  33	0.001384
  34	0.001399
  35	0.001532
  36	0.001582
  37	0.001507
  38	0.001552
  39	0.001581
  40	0.001712
  41	0.001614
  42	0.001706
  43	0.001881
  44	0.001816
  45	0.001668
  46	0.001739
  47	0.001806
  48	0.001754
  49	0.001825
  50	0.001802
  51	0.001814
  52	0.001881
  53	0.001883
  54	0.001952
  55	0.001842
  56	0.001935
  57	0.001937
  58	0.001929
  59	0.001900
  60	0.001907
  61	0.001891
  62	0.001852
  63	0.001887
  64	0.001901
  ```

</details>

## Экспериментальные данные

### Тип разделения: static

#### Зависимость времени от количества потоков

![image](assets/static/The%20dependence%20of%20the%20operating%20time%20on%20the%20number%20of%20threads.svg)

#### Зависимость ускорения от количества потоков

![image](assets/static/The%20dependence%20of%20acceleration%20on%20the%20number%20of%20threads.svg)

#### Зависимость эффективности работы программы от количества потоков

![image](assets/static/The%20dependence%20of%20efficiency%20on%20the%20number%20of%20threads.svg)

### Тип разделения: dynamic

#### Зависимость времени от количества потоков

![image](assets/dynamic/The%20dependence%20of%20the%20operating%20time%20on%20the%20number%20of%20threads.svg)

#### Зависимость ускорения от количества потоков

![image](assets/dynamic/The%20dependence%20of%20acceleration%20on%20the%20number%20of%20threads.svg)

#### Зависимость эффективности работы программы от количества потоков

![image](assets/dynamic/The%20dependence%20of%20efficiency%20on%20the%20number%20of%20threads.svg)

### Тип разделения: guided

#### Зависимость времени от количества потоков

![image](assets/guided/The%20dependence%20of%20the%20operating%20time%20on%20the%20number%20of%20threads.svg)

#### Зависимость ускорения от количества потоков

![image](assets/guided/The%20dependence%20of%20acceleration%20on%20the%20number%20of%20threads.svg)

#### Зависимость эффективности работы программы от количества потоков

![image](assets/guided/The%20dependence%20of%20efficiency%20on%20the%20number%20of%20threads.svg)

## Заключение

В рамках проведенного исследования, в котором применялись различные типы разделения нагрузки, было выявлено следующее:

- Тип разделения: **static**

  Похожий тренд, как и в предыдущем случае. Результаты для разных размеров фрагментов (100 и 10000) схожи, что говорит о
  стабильности этого типа разделения.

- Тип разделения: **dynamic**

  Здесь время также уменьшается с увеличением количества потоков, но стабильность и предсказуемость результатов меньше,
  особенно при малых размерах фрагментов (100). Это может быть связано с более высокими накладными расходами на
  динамическое распределение работы между потоками.

- Тип разделения: **guided**

  Время сначала уменьшается, достигая минимума, а затем увеличивается с увеличением количества потоков. Этот тип
  показывает лучшие результаты при меньшем количестве потоков по сравнению с другими типами разделения, что может быть
  связано с более эффективным распределением работы в начале выполнения.

## Приложение

### Программа для анализа окружения

<details>
  <summary>Исходный код программы для анализа окружения</summary>

```c++
#include <stdio.h>
#include <omp.h>

int main() {
    printf("OpenMP Version %d\n", _OPENMP);
    printf("Number of processors: %d\n", omp_get_num_procs());
    printf("Maximum number of threads: %d\n", omp_get_max_threads());
    printf("Dynamic threads are %s\n", omp_get_dynamic() ? "enabled" : "disabled");
    printf("Timer resolution: %g seconds\n", omp_get_wtick());
    printf("Nested parallelism is %s\n", omp_get_nested() ? "enabled" : "disabled");
    printf("Max active levels: %d\n", omp_get_max_active_levels());

    omp_sched_t kind;
    int chunk_size;
    omp_get_schedule(&kind, &chunk_size);
    printf("Schedule: %d, chunk size: %d\n", kind, chunk_size);

    return 0;
}
```

</details>

### Параллельная программа, использующая механизм явных блокировок

<details>
  <summary>Исходный код параллельной программы, использующей механизм явных блокировок</summary>

```c++
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    const int count = 100;
    const int random_seed = 1337;

    srand(random_seed);

    int sum = 0;
    int *array = malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) { array[i] = rand(); }

    omp_lock_t lock;
    omp_init_lock(&lock);

#pragma omp parallel for
    for (int i = 0; i < count; i++) {
        omp_set_lock(&lock);
        sum += array[i];
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);

    printf("Sum: %d\n", sum);

    free(array);
    return 0;
}
```

</details>

### Параллельная программа с настройкой schedule

<details>
  <summary>Исходный код параллельной программы с настройкой schedule, количество потоков от 1 до 64</summary>

```c++
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    const int count = 10000000;
    const int random_seed = 1337;
    const int iterations = 25;
    const int max_threads = 64;
    const int chunk = 100;

    srand(random_seed);

    int max;
    int *array = malloc(count * sizeof(int));

    double start_time, end_time, total;

    printf("Threads\tTime\n");
    for (int threads = 1; threads <= max_threads; threads++) {
        total = 0;

        for (int j = 0; j < iterations; j++) {
            for (int i = 0; i < count; i++) { array[i] = rand(); }
            max = -1;
            start_time = omp_get_wtime();
#pragma omp parallel num_threads(threads) shared(array, count, chunk) reduction(max: max) default(none)
            {
#pragma omp for schedule(static, chunk)
                for (int i = 0; i < count; i++) {
                    if (array[i] > max) {
                        max = array[i];
                    }
                }
            }
            end_time = omp_get_wtime();
            total += end_time - start_time;
        }
        printf("%d\t%f\n", threads, total / (double) iterations);
    }

    free(array);
    return 0;
}
```

</details>
