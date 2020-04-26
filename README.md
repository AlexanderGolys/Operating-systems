# Operating-systems

## Projects included:

### Lab3: Threads synchronization using semaphores on Linux.
Napisz w języku C++ w środowisku Linux-a program który ukaże przykład rozwiązania problemu synchronizacji procesów wykorzystując semafory. 

Zadanie:

W systemie są dwie kolejki FIFO o maksymalnym rozmiarze S które przechowują wiadomości o następującej strukturze – id kolejki, id producenta, id wiadomości. Na kolejkach operują trzy procesy:

a) czytelnicy - procesy te sparametryzowane są id kolejki na której pracują; procesy te oczekują na pojawienie się wiadomości w swojej kolejce, gdy taka się pojawi wypisują jej treść (bez usuwania wiadomości z kolejki) a następnie zasypiają na losowy okres czasu. Dopuszczalne jest aby wiele procesów tego typu operowało jednocześnie na kolejce a także by procesy te wielokrotnie wypisały ta sama wiadomość. Nie jest natomiast dopuszczalne aby procesy te czytały zawartość kolejki w sytuacji gdy operuje na niej proces którejś zpozostałych kategorii,

b) pisarze-producenci - co losowy okres czasu tworzą i umieszczają w jednej z kolejek nowa wiadomość. Jeżeli proces wyprodukuje wiadomość a kolejka w której zamierzał ja umieścić jest pełna, to proces taki oczekuje aż w kolejce powstanie wolne miejsce,

c) pisarze-konsumenci - co losowy okres czasu zabierają po jednej wiadomości z każdej kolejki - jeżeli w którejś z kolejek nie ma wiadomości to proces ten oczekuje na pojawienie sie nowej wiadomości.

Należy zapewnić sprawne działanie systemu (bez zakleszczeń oraz zagłodzenia któregoś z procesów, zaginięcia czy tez zduplikowania wiadomości) w przypadku gdy występuje N procesów klasy pierwszej, Mprocesów klasy drugiej i P procesów klasy trzeciej!
