Γεώργιος Μάνος - Α.Μ.: 4333

Η υλοποίηση μου ακολουθεί ακριβώς τα βήματα της εκφώνησης.

Οι αλλαγές που κάνουμε στα αρχεία του Linux είναι ακριβώς ανάλογες με το dummy παράδειγμα του φροντιστηρίου.
Στην υλοποίηση των system calls set και get ωστόσο, στην set ελέγχω απλώς αν οι αριθμοί είναι λογικοί (μη -
αρνητικοί και η περίοδος (σε ms) είναι μεγαλύτερη από τον χρόνο εκτέλεσης (σε ms). Η get ωστόσο ελέγχει απλώς
αν είναι Null ο pointer ορίσματος και στην συνέχεια γεμίζει το struct του κατάλληλα με τις τιμές που υπάρχουν
στο period_params struct του task_struct της διεργασίας που επέστρεψε η get_current (εν προκειμένω το test πρό-
γραμμα μας). 

Η προσθήκη έγινε ως εξής, κάναμε αρχικά define τους αριθμούς των νέων system calls στο unistd_32.h και αυξήσαμε
τον συνολικό αριθμό των system calls κατά 2. Στην συνέχεια δίνουμε την πληροφορία στον kernel για τα νέα system
calls μέσω του syscall_table_32.S (όπου προσθέτουμε 2 entries το τέλος της λίστας). Μετά επεξεργαζόμαστε το 
syscalls.h προσθέτοντας το signature του κάθε system call, και τέλος δημιουργούμε 2 .c αρχεία, ένα για το κάθε
system call μέσα στον kernel με τον ορισμό της αντίστοιχης συνάρτησης.

Για να τα χρησιμοποιήσουμε στον qemu εφόσον χρησιμοποιήσουμε το image που παράχθηκε από το compilation του kernel
απλώς προσθέσαμε στο unistd.h (ως super user) το definition του struct period_params και τίποτα άλλο (για αυτό και
το παραδίδω μαζί με τα testfiles μου, τα οποία το ένα ελέγχει εάν τρέχουν σωστά τα system calls και το άλλο ελέγχει
εάν πιάνουν τα λάθη. Το Makefile τους ειναι ένα απλό gcc)