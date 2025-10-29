#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Employee structure
typedef struct Employee {
    int id;
    char name[50];
    char role[50];
    struct Employee *next;
    struct Employee *supervisor;
    struct Employee *secondaryBoss;
} Employee;

// Create a new employee node
Employee* createEmployee(int id, const char *name, const char *role) {
    Employee *newEmp = (Employee*) malloc(sizeof(Employee));
    newEmp->id = id;
    strcpy(newEmp->name, name);
    strcpy(newEmp->role, role);
    newEmp->next = NULL;
    newEmp->supervisor = NULL;
    newEmp->secondaryBoss = NULL;
    return newEmp;
}

// Insert employee at end
void insertEmployee(Employee **head, int id, const char *name, const char *role) {
    Employee *newEmp = createEmployee(id, name, role);

    if (*head == NULL) {
        *head = newEmp;
        printf("Employee %d (%s - %s) added as department head.\n", id, name, role);
        return;
    }

    Employee *temp = *head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newEmp;
    newEmp->supervisor = temp;
    printf("Employee %d (%s - %s) added under supervisor %d (%s).\n",
           id, name, role, temp->id, temp->name);
}

// Promote employee (move one step closer to head)
void promoteEmployee(Employee **head, int id) {
    if (*head == NULL || (*head)->next == NULL) {
        printf("No employees to promote.\n");
        return;
    }

    Employee *prev = NULL, *curr = *head;

    // Find employee
    while (curr && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr || !prev) {
        printf("Promotion not possible for ID %d (not found or already at top).\n", id);
        return;
    }

    // Swap nodes
    prev->next = curr->next;
    curr->next = prev;

    // Fix supervisor pointers
    curr->supervisor = prev->supervisor;
    prev->supervisor = curr;

    if (*head == prev)
        *head = curr;

    printf("Employee %d (%s) promoted successfully.\n", id, curr->name);
}

// Reassign supervisor
void reassignEmployee(Employee *head, int empID, int newSupID) {
    if (head == NULL) {
        printf("No employees found.\n");
        return;
    }

    Employee *emp = head, *newSup = head;

    // Find employee
    while (emp && emp->id != empID)
        emp = emp->next;

    // Find new supervisor
    while (newSup && newSup->id != newSupID)
        newSup = newSup->next;

    if (!emp) {
        printf("Employee with ID %d not found.\n", empID);
        return;
    }

    if (!newSup) {
        printf("Supervisor with ID %d not found.\n", newSupID);
        return;
    }

    emp->supervisor = newSup;
    printf("Employee %d (%s) now reports to %d (%s).\n",
           emp->id, emp->name, newSup->id, newSup->name);
}

// Assign matrix (secondary) supervisor
void assignMatrixSupervisor(Employee *head, int empID, int bossID) {
    if (head == NULL) {
        printf("No employees found.\n");
        return;
    }

    Employee *emp = head, *boss = head;

    while (emp && emp->id != empID)
        emp = emp->next;
    while (boss && boss->id != bossID)
        boss = boss->next;

    if (!emp) {
        printf("Employee with ID %d not found.\n", empID);
        return;
    }

    if (!boss) {
        printf("Secondary Supervisor with ID %d not found.\n", bossID);
        return;
    }

    emp->secondaryBoss = boss;
    printf("Employee %d (%s) also reports to %d (%s) (matrix structure).\n",
           emp->id, emp->name, boss->id, boss->name);
}

// Display department
void displayDepartment(Employee *head) {
    printf("\n--- Department Structure ---\n");
    if (head == NULL) {
        printf("No employees in the department.\n");
        return;
    }

    while (head) {
        printf("ID:%d, Name:%s, Role:%s", head->id, head->name, head->role);
        if (head->supervisor)
            printf(", Supervisor:%s", head->supervisor->name);
        if (head->secondaryBoss)
            printf(", SecondaryBoss:%s", head->secondaryBoss->name);
        printf("\n");
        head = head->next;
    }
    printf("------------------------------\n");
}

// Main function
int main() {
    Employee *department = NULL;
    int choice, id, supID;
    char name[50], role[50];

    while (1) {
        printf("\n=== Employee Promotion & Reporting System ===\n");
        printf("1. Add Employee\n");
        printf("2. Promote Employee\n");
        printf("3. Reassign Supervisor\n");
        printf("4. Assign Matrix Supervisor\n");
        printf("5. Display Department\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Employee ID: ");
                scanf("%d", &id);
                printf("Enter Employee Name: ");
                scanf("%s", name);
                printf("Enter Employee Role: ");
                scanf("%s", role);
                insertEmployee(&department, id, name, role);
                break;

            case 2:
                printf("Enter Employee ID to promote: ");
                scanf("%d", &id);
                promoteEmployee(&department, id);
                break;

            case 3:
                printf("Enter Employee ID: ");
                scanf("%d", &id);
                printf("Enter New Supervisor ID: ");
                scanf("%d", &supID);
                reassignEmployee(department, id, supID);
                break;

            case 4:
                printf("Enter Employee ID: ");
                scanf("%d", &id);
                printf("Enter Secondary Supervisor ID: ");
                scanf("%d", &supID);
                assignMatrixSupervisor(department, id, supID);
                break;

            case 5:
                displayDepartment(department);
                break;

            case 6:
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
