#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
 #define SAVE_FILE "patients_data.bin"
 #include <windows.h>

typedef enum {
    WAITING,UNDER_TRAITMENT,DISCHARGED
}pateinstaus;
typedef enum {
    CARDIOLOGY,
    PEDIATRICS,
    NEUROLOGY,
    ORTHOPEDICS,
    GENERAL 
} Department; 
typedef struct pateint{ 
    int id;
    char name[50];
    char gender[10];
    int age;
    Department departement;
    char malade[40];
    pateinstaus status;
    int id_doctor;

}pateints;

typedef struct {
    int id;
    char name[60];
    Department department;
    bool available;
} DOCTOR;
typedef struct stack {
    pateints patient; 
    struct stack* next;
} stack; 
typedef struct { 
    stack* top;
    int count;
}disharge_p;
typedef struct QueueNode {
    pateints patient;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int count;
} PatientQueue;


typedef struct Tree {
    pateints patient; 
    struct Tree* left;
    struct Tree*  right; 
} Tree;

int count=0;
pateints malades[150];
DOCTOR doctors[60];
int count_d=0;
disharge_p discharge_patient={
    NULL,0
};
PatientQueue waiting_treatment ={NULL, NULL, 0};
Tree* patient_tree = NULL;


void menu_main();
void edit_patient(int id);
void add_patients();
void removepatient(int id);
void view_patient(int id);
void view_patients();
void manage_patients();
void add_doctor();
void edit_doctor(int id);
void remove_doctor(int id);
void view_doctor(int id);
void view_doctors();
void manage_doctor();
void discharge_malade (int id);
void add_p_queue(int id);
void view_hospital_structure();
void view_waiting_queue();
 void add_p_queue(int id) ;
 void undo_last_discharge();
void save_data_to_file();
void load_data_from_file();
void search_patient_to_tree(int id);
void insert_patient_to_tree(Tree** root, pateints patient);
Tree* search_patient(Tree* root, int id) ;
void menu_main(){
    printf("\n=== HOSPITAL PATIENT MANAGEMENT SYSTEM ===\n");
    printf("1. Manage Patients\n");
    printf("2. Manage Doctors\n");
    printf("3. Discharge Patient\n");
    printf("4. View Waiting Queue\n");
    printf("5. Add Patient to Queue\n");
    printf("6. Undo Last Discharge\n");
    printf("7. Search Patient in Directory Tree\n");
    printf("8. View Hospital Structure Tree\n");
    printf("9. Save Data to File\n");
    printf("10. Load Data from File\n");
    printf("11. Exit\n");
    printf("Choose an option: ");
}
void afficher_patients_par_departement(Department dept) ;
void afficher_patients_par_departement_rec(Tree* node, Department dept);
void rechercher_et_afficher_patient_par_namep(Tree* patient_tree, char* nom_partiel) ;
void rechercher_et_afficher_patient_namep_rec(Tree* node, const char* nom_partiel, int* found);
void free_tree(Tree* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

void free_queue(QueueNode* node) {
    while (node != NULL) {
        QueueNode* temp = node;
        node = node->next;
        free(temp);
    }
}
void free_stack(stack* node) {
    while (node != NULL) {
        stack* temp = node;
        node = node->next;
        free(temp);
    }
}   
void lookdepartement(Department dept) {
    switch(dept) {
        case CARDIOLOGY:   printf("Cardiology"); break;
        case PEDIATRICS:   printf("Pediatrics"); break;
        case NEUROLOGY:    printf("Neurology"); break;
        case ORTHOPEDICS:  printf("Orthopedics"); break;
        case GENERAL:     printf("General Medicine"); break;
        default:          printf("Unknown Department"); break;
    }
}

int main(){

    int choice;
    do {
       menu_main();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: manage_patients (); break; 
            
            case 2: manage_doctor(); break; 
            case 3: {
                int id;
                printf("Enter patient ID to discharge: ");
                scanf("%d", &id);
                discharge_malade(id);
                break;
            }
            case 4: view_waiting_queue(); break;
            case 5: {
                int id;
                printf("Enter patient ID to add to queue: ");
                scanf("%d", &id);
                  add_p_queue(id);
                break;
            }
            case 6: undo_last_discharge(); break;
            case 7: {
                int id;
                printf("Enter patient ID to search: ");
                scanf("%d", &id);
                search_patient_to_tree(id);
                break;
            }
            case 8: view_hospital_structure(); break;
            case 9: save_data_to_file(); break;
            case 10: load_data_from_file(); break;
            case 11: printf("Exiting system...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while(choice != 11);

return 0;}



void manage_patients(){
    int choice;
    do {
        printf("\n=== PATIENT MANAGEMENT ===\n");
        printf("1. Add Patient\n");
        printf("2. Edit Patient\n");
        printf("3. View Patient\n");
        printf("4. Delete Patient\n");
        printf("5. View All Patients\n");
        printf("6-afficher_patients_par_departement:\n");
        printf("7-search by closed name:\n");
        printf("8. Back to Main Menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: add_patients(); break;
            case 2: {
                int id;
                printf("Enter patient ID to edit: ");
                scanf("%d", &id);
                edit_patient(id);
                break;
            }
            case 3: {
                int id;
                printf("Enter patient ID to view: ");
                scanf("%d", &id);
                view_patient(id);
                break;
            }
            case 4: {
                int id;
                printf("Enter patient ID to delete: ");
                scanf("%d", &id);
                removepatient(id);
                break;
            }
            case 5: view_patients(); break;
            case 6: {
                printf("Enter department number (0-4):\n");
                printf("0: Cardiology\n1: Pediatrics\n2: Neurology\n3: Orthopedics\n4: General\n");
                int dept_num;
                scanf("%d", &dept_num);
                if (dept_num >= 0 && dept_num <= 4) {
                    afficher_patients_par_departement((Department)dept_num);
                } else {
                    printf("Invalid department number!\n");
                }
                break;
            }
            case 7: {
                char nom_partiel[50];
                printf("Entrez une partie du nom du patient : ");
                scanf(" %[^\n]", nom_partiel);
                rechercher_et_afficher_patient_par_namep(patient_tree, nom_partiel);
                break;
            }
            
            case 8 :break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while(choice != 8);}
    void add_patients(){
        pateints p;
        if(count>=149){ printf("the hospital is full."); return;}
        
        printf("enter name for patient:\n");
        getchar();
        scanf("%[^\n]",p.name);
        printf("enter id:\n");
        scanf("%d",&p.id);
        printf("enter the age:\n");
        scanf("%d",&p.age);
        printf("enter patient gender:\n");
        scanf("%s",p.gender);
        printf("enter patient illness:\n");
        scanf("%s",p.malade);
        
        printf("Enter department (0-Cardiology, 1-Pediatrics, 2-Neurology, 3-Orthopedics, 4-General): ");
        int dept;
        scanf("%d", &dept);
        p.departement = (Department)dept;
        
        p.status = WAITING;
        p.id_doctor=-1;
        malades[count++] = p;
        insert_patient_to_tree(&patient_tree, p);
        
        printf("Patient added successfully!\n");
    }
void edit_patient(int id){
    
    
    for(int i=0;i<count;i++){
        if(malades[i].id==id){
            printf("enter new name for(curent name:%s)",malades[i].name);
            getchar();
            scanf("%[^\n]",malades[i].name);
         printf("enter new id (curent id=%d)",malades[i].id);
         scanf("%d",&malades[i].id);
         printf("enter new age (curent age:%d)",malades[i].age);
         scanf("%d",&malades[i].age);
         printf("enter new gender (curent gender is:%s)",malades[i].gender);
         getchar();
         scanf("%s",malades[i].gender);
         printf("enter new ailement (curent ailement :%s)",malades[i].malade);
         scanf("%[^\n]",malades[i].malade);
         printf("pateints updated successfully");
         return;
            }
            
        }
        printf("pateints with id (%d ) dont found \n",id);
    }
  void removepatient(int id) {
    bool isexist = false;

    for (int i = 0; i < count; i++) {
        if (malades[i].id == id) {
            for (int j = i; j < count - 1; j++) {
                malades[j] = malades[j + 1];
            }
            count--;
            isexist = true;
            break; 
        }
    }

    if (isexist) {
        printf("Patient with ID %d removed successfully.\n", id);
    } else {
        printf("Patient with ID %d not found.\n", id);
    }
}

   void view_patients(){
    for(int i=0;i<count;i++){
        printf("the information for sick %d :\n",i+1);
        printf("name:%s ",malades[i].name);
        printf("id:%d ",malades[i].id);
        printf("gender:%s ",malades[i].gender);
        printf("illness:%s ",malades[i].malade);
        printf("the status: ");
        switch (malades[i].status) {
            case WAITING: printf("WAITING\n"); break;
            case UNDER_TRAITMENT: printf("UNDER TRAITMENT\n"); break;
            case DISCHARGED: printf("DISCHARGED\n"); break;
            default: printf("UNKNOWN\n");
        }

    }
   }
   void view_patient(int id){
    for(int i=0;i<count;i++){
        if(malades[i].id==id){
            printf("\n==pateints information==\n",id);
            printf("ID: %d\n", malades[i].id);
            printf("Name: %s\n", malades[i].name);
            printf("Gender: %s\n", malades[i].gender);
            printf("Age: %d\n", malades[i].age);
            printf("Illness: %s\n", malades[i].malade);
            printf("Doctor ID: %d\n", malades[i].id_doctor);
            printf("Status: ");
            switch (malades[i].status) {
                case WAITING: printf("WAITING\n"); break;
                case UNDER_TRAITMENT: printf("UNDER TRAITMENT\n"); break;
                case DISCHARGED: printf("DISCHARGED\n"); break;
                default: printf("UNKNOWN\n");
            }
        }
    }
   }
   void manage_doctor(){
    printf("\n==HOSPITAL MANAGE DOCTORS==\n");
    int choice;
    do {
        printf("\n=== DOCTOR MANAGEMENT ===\n");
        printf("1. Add Doctor\n");
        printf("2. Edit Doctor\n");
        printf("3. View Doctor\n");
        printf("4. Delete Doctor\n");
        printf("5. View All Doctors\n");
        printf("6. Back to Main Menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: add_doctor(); break;
            case 2: {
                int id;
                printf("Enter doctor ID to edit: ");
                scanf("%d", &id);
                edit_doctor(id);
                break;
            }
            case 3: {
                int id;
                printf("Enter doctor ID to view: ");
                scanf("%d", &id);
                view_doctor(id);
                break;
            }
            case 4: {
                int id;
                printf("Enter doctor ID to delete: ");
                scanf("%d", &id);
                remove_doctor(id);
                break;
            }
            case 5: view_doctors(); break;
            case 6: break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while(choice != 6);
}
void add_doctor(){
    DOCTOR d;
    if (count_d>=60) {
        printf("Maximum doctor capacity reached!\n");
        return;
    }

    
    printf("Enter doctor name: ");
    scanf(" %[^\n]", d.name);
    
    printf("Enter doctor ID: ");
   scanf("%d", &d.id);
        
        printf("Enter department (0-Cardiology, 1-Pediatrics, 2-Neurology, 3-Orthopedics, 4-General): ");
        int dept;
        scanf("%d", &dept);
        d.department=(Department)dept;
         d.available = true;
        
        doctors[count_d++] = d;
        printf("Doctor added successfully!\n");
    }
    void edit_doctor(int id){
        bool isexist=false;
        for(int i=0;i<count_d;i++){
            if(doctors[i].id==id){
                printf("enter new name:(curent name:%s)\n",doctors[i].name);
                scanf("%[^\n]",doctors[i].name);
                printf("enter new id (curent id :%d)\n",doctors[i].id);
                scanf("%d",&doctors[i].id);
                printf("enter departement (curent departemrnt: ");
        switch(doctors[i].department){
            case  CARDIOLOGY: printf("CARDIOLOGY )"); break;
          case  PEDIATRICS: printf("PEDIATRICS )"); break;
          case  NEUROLOGY: printf("NEUROLOGY ) ");break;
          case  ORTHOPEDICS : printf("ORTHOPEDICS )"); break;
          case  GENERAL : printf("GENERAL )"); break;
          default: printf("UNKNOWN )");break;

            
        }
        scanf("%d",&doctors[i].department);
        isexist=true;
        printf("the doctor updated succssefully");
        return;
            }
        }
    
            printf("the doctor of id=%d not faund .",id);
    
        }

    void remove_doctor(int id){
        for (int i = 0; i <count_d; i++) {
            if (doctors[i].id == id) {
            
                for (int j = i; j < count_d - 1; j++) {
                    doctors[j] = doctors[j + 1];
                }
                count_d--;
                
                printf("Doctor removed successfully!\n");
                return;
            }
        }
        printf("Doctor with ID %d not found!\n", id);
    }
    void view_doctor(int id){
        for(int i=0;i<count_d;i++){
            if(doctors[i].id==id){
                printf("\n==the doctor with id =%d :==",id);
                printf("name :%s\n",doctors[i].name);
                printf("id :%d\n",doctors[i].id);
                printf("departement:");
                lookdepartement(doctors[i].department);
                printf("\navailable : %s\n",doctors[i].available ? "YES": "NO");
            }
        }
    }
    void view_doctors(){
        printf("\n==doctors in hospital:==\n");
        for(int i=0;i<count_d;i++){
        printf("name: %s\n",doctors[i].name);
        printf("id :%d\n",doctors[i].id);
        printf("departement:");
      lookdepartement(doctors[i].department);
        printf("\navailable :%s\n",doctors[i].available ? "YES" :"NO");

        }
    }
    void discharge_malade(int id) {
        for (int i = 0; i < count; i++) {
            if (malades[i].id == id && malades[i].status != DISCHARGED) { 
                malades[i].status = DISCHARGED;

                stack* newnode = (stack*)malloc(sizeof(stack));  
                newnode->patient = malades[i]; 
                newnode->next =discharge_patient.top;
                discharge_patient.top = newnode;
                discharge_patient.count++; 
                
                printf("Patient %s (ID: %d) discharged successfully!\n", 
                    malades[i].name, malades[i].id);
                return;
            }
        }
        printf("Patient with ID %d not found or already discharged!\n", id);
    }
    void undo_last_discharge() {
        if (discharge_patient.count == 0) {
            printf("No discharges to undo!\n");
            return;
        }
        
        stack* top= discharge_patient.top;
        pateints p = top->patient;
        
        for (int i = 0; i < count; i++) {
            if (malades[i].id == p.id) {
                malades[i].status = UNDER_TRAITMENT;
                break;
            }
        }
        
    
        discharge_patient.top = top->next;
        free(top);
        discharge_patient.count--;
        
        printf("Undo successful! Patient %s (ID: %d) status reverted.\n", 
              p.name, p.id);
    }
    void view_waiting_queue() {
        printf("\n=== WAITING QUEUE ===\n");
        if (waiting_treatment.count == 0) {
            printf("Queue is empty.\n");
            return;
        }
        
        QueueNode* current = waiting_treatment.front;
        int position = 1;
        while (current != NULL) {
            printf("%d. %s (ID: %d)\n", position++, current->patient.name, current->patient.id);
            current = current->next;
        }
    }
    void add_p_queue(int id) {
        for (int i = 0; i < count; i++) {
            if (malades[i].id == id && malades[i].status == WAITING) {
                QueueNode* newnode = (QueueNode*)malloc(sizeof(QueueNode));
                newnode->patient = malades[i];
                newnode->next = NULL;
    
                if (waiting_treatment.rear == NULL) {
                    waiting_treatment.front = waiting_treatment.rear = newnode;
                } else {
                    waiting_treatment.rear->next = newnode;
                    waiting_treatment.rear = newnode;
                }
                waiting_treatment.count++;
                malades[i].status = UNDER_TRAITMENT; 
                printf("Patient %s added to the queue.\n", malades[i].name);
                return;
            }
        }
        printf("Patient not found or not in WAITING status.\n");
    }
 

    void rechercher_et_afficher_patient_par_namep(Tree* patient_tree, char* nom_partiel) {
        if (patient_tree == NULL || nom_partiel == NULL || strlen(nom_partiel) == 0) {
            printf("Nom invalide ou arbre vide.\n");
            return;
        }
    
        int found = 0;
        printf("Recherche des patients correspondant au nom partiel \"%s\" :\n", nom_partiel);
        printf("====================================================\n");
        rechercher_et_afficher_patient_namep_rec(patient_tree, nom_partiel, &found);
    
        if (!found) {
            printf("Aucun patient trouvé avec ce nom partiel.\n");
        }
    }
    void rechercher_et_afficher_patient_namep_rec(Tree* node, const char* nom_partiel, int* found) {
        if (node == NULL) {
            return;
        }
    
        
        if (strstr(node->patient.name, nom_partiel) != NULL) {
            printf("Nom: %s, ID: %d, Département: %d \n", node->patient.name, node->patient.id, node->patient.departement);
            *found = 1; 
        }
    
        
        rechercher_et_afficher_patient_namep_rec(node->left, nom_partiel, found);
        rechercher_et_afficher_patient_namep_rec(node->right, nom_partiel, found);
    }
        
    
    void afficher_patients_par_departement_rec(Tree* node, Department dept){
          if (node == NULL) return;
        if (node->patient.departement == dept) {
            printf("ID: %-6d | Name: %-20s | Status: ", node->patient.id, node->patient.name);
            switch(node->patient.status) {
                case WAITING: printf("Waiting"); break;
                case UNDER_TRAITMENT: printf("Under Treatment"); break;
                case DISCHARGED: printf("Discharged"); break;
            }
            printf(" Departement:");
            lookdepartement(node->patient.departement); 
            printf("\n");
        }
    
        //
        afficher_patients_par_departement_rec(node->left, dept);
        afficher_patients_par_departement_rec(node->right, dept);
    }
    void afficher_patients_par_departement(Department dept) {
        
        printf("\nPatients in department: ");
        lookdepartement(dept);
        printf("\n");
        printf("==============================\n");
        
        if (patient_tree == NULL) {
            printf("No patients found in this department.\n");
            return;
        }
        
        afficher_patients_par_departement_rec(patient_tree, dept);
    }

    
    void insert_patient_to_tree(Tree** root, pateints patient) {
        if (*root == NULL) {
            Tree* new_node = (Tree*)malloc(sizeof(Tree));
            new_node->patient = patient;
            new_node->left = new_node->right = NULL;
            *root = new_node;
            return;
        }
    
        if (patient.id < (*root)->patient.id)
            insert_patient_to_tree(&((*root)->left), patient);
        else
            insert_patient_to_tree(&((*root)->right), patient);
    }
    
    void search_patient_to_tree(int id ){ //bst to find pateints by id
        Tree* result = search_patient(patient_tree, id);
        if (result != NULL) {
            printf("Patient found in directory:\n");
            printf("ID: %d, Name: %s\n", result->patient.id, result->patient.name);
        } else {
            printf("Patient with ID %d not found in directory tree.\n", id);
        }
    }
    
    void view_hospital_structure() {
        printf("\n=== HOSPITAL STRUCTURE ===\n");
        printf("├── Cardiology Department\n");
        printf("│   ├── Outpatient Services\n");
        printf("│   └── Emergency Services\n");
        printf("└── Pediatrics Department\n");
        printf("    ├── Inpatient Services\n");
        printf("    └── NICU Team\n");
    }
    
     
    void save_data_to_file() {
    FILE* bin_file = fopen("patients_data.bin", "wb");
    if (!bin_file) {
        printf("Erreur lors de l'ouverture du fichier binaire pour l'écriture.\n");
        return;
    }

    // Patients
    fwrite(&count, sizeof(int), 1, bin_file);
    fwrite(malades, sizeof(pateints), count, bin_file);

    // Doctors
    fwrite(&count_d, sizeof(int), 1, bin_file);
    fwrite(doctors, sizeof(DOCTOR), count_d, bin_file);

    // Queue patients
    int queueCount = waiting_treatment.count;
    fwrite(&queueCount, sizeof(int), 1, bin_file);
    QueueNode* qnode = waiting_treatment.front;
    while (qnode) {
        fwrite(&qnode->patient, sizeof(pateints), 1, bin_file);
        qnode = qnode->next;
    }

    // Stack patients
    int stackCount = discharge_patient.count;
    fwrite(&stackCount, sizeof(int), 1, bin_file);
    stack* snode = discharge_patient.top;
    while (snode) {
        fwrite(&snode->patient, sizeof(pateints), 1, bin_file);
        snode = snode->next;
    }

    fclose(bin_file);

    // Fichier texte lisible
    FILE* text_file = fopen("patients_data.txt", "w");
    if (text_file) {
        for (int i = 0; i < count; i++) {
            fprintf(text_file, "ID: %d, Nom: %s, Age: %d, Statut: %d, Departement: %d\n",
                    malades[i].id, malades[i].name, malades[i].age,
                    malades[i].status, malades[i].departement);
        }
        fclose(text_file);
    }

    printf("Données enregistrées avec succès.\n");
}

   void load_data_from_file() {
     FILE* file = fopen(SAVE_FILE, "rb");
      if (file == NULL) { 
        printf("Error opening file for loading!\n");
         return; }
         free_tree(patient_tree);
          patient_tree = NULL;
           free_queue(waiting_treatment.front); 
           waiting_treatment.front = waiting_treatment.rear = NULL;
            waiting_treatment.count = 0;
            free_stack(discharge_patient.top); 
            discharge_patient.top = NULL;
             discharge_patient.count = 0;
              fread(&count, sizeof(int), 1, file);
               fread(malades, sizeof(pateints), count, file); 
               for (int i = 0; i < count; i++) { insert_patient_to_tree(&patient_tree, malades[i]); } 
               fread(&count_d, sizeof(int), 1, file); fread(doctors, sizeof(DOCTOR), count_d, file);
                int queueCount; 
                fread(&queueCount, sizeof(int), 1, file);
                 for (int i = 0; i < queueCount; i++) { pateints p; fread(&p, sizeof(pateints), 1, file);
                     QueueNode* newnode = (QueueNode*)malloc(sizeof(QueueNode));
                          newnode->patient = p; newnode->next = NULL; 
                      if (waiting_treatment.rear == NULL) { waiting_treatment.front = waiting_treatment.rear = newnode; }
                       else { waiting_treatment.rear->next = newnode; waiting_treatment.rear = newnode; } waiting_treatment.count++; }
                        int stackCount; fread(&stackCount, sizeof(int), 1, file); 
                        for (int i = 0; i < stackCount; i++) { pateints p; fread(&p, sizeof(pateints), 1, file); 
                        stack* newnode = (stack*)malloc(sizeof(stack)); 
                        newnode->patient = p;
          newnode->next = discharge_patient.top;
           discharge_patient.top = newnode;
         discharge_patient.count++; }
   fclose(file);
     printf("Data loaded successfully from %s\n", SAVE_FILE); 
     view_patients(); }
    Tree* search_patient(Tree* root, int id) {
        if (root == NULL || root->patient.id == id) {
            return root;
        }
        if (id < root->patient.id) {
            return search_patient(root->left, id);
        } else {
            return search_patient(root->right, id);
        }
    }
