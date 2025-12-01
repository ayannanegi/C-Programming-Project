#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FILE_NAME "tasks.txt"

typedef struct {
    int day, month, year;
    int hour, minute;
} Deadline;

typedef struct {
    int id;
    char title[100];
    char priority[20];
    Deadline deadline;
    int completed;
} Task;

Task tasks[MAX];
int count = 0;


// Save all tasks back to file
void saveTasksToFile() {
    FILE *fp = fopen(FILE_NAME, "w");
    if (!fp) {
        printf("Could not open file to save.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%d-%d-%d %d:%d|%d\n",
            tasks[i].id,
            tasks[i].title,
            tasks[i].priority,
            tasks[i].deadline.day,
            tasks[i].deadline.month,
            tasks[i].deadline.year,
            tasks[i].deadline.hour,
            tasks[i].deadline.minute,
            tasks[i].completed
        );
    }

    fclose(fp);
}


// To load tasks when the program starts
void loadTasksFromFile() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return;

    while (!feof(fp)) {
        Task t;
        int comp;

        if (fscanf(
                fp,
                "%d|%99[^|]|%19[^|]|%d-%d-%d %d:%d|%d\n",
                &t.id,
                t.title,
                t.priority,
                &t.deadline.day,
                &t.deadline.month,
                &t.deadline.year,
                &t.deadline.hour,
                &t.deadline.minute,
                &comp
            ) == 9) 
        {
            t.completed = comp;
            tasks[count++] = t;
        }
    }

    fclose(fp);
}


// To add a task
void addTask() {
    Task t;
    t.id = (count == 0) ? 1 : tasks[count - 1].id + 1;

    printf("\nTitle: ");
    scanf(" %[^\n]", t.title);

    printf("Priority (High/Medium/Low): ");
    scanf("%s", t.priority);

    printf("Deadline (DD MM YYYY HH MM): ");
    scanf("%d %d %d %d %d",
        &t.deadline.day,
        &t.deadline.month,
        &t.deadline.year,
        &t.deadline.hour,
        &t.deadline.minute
    );

    t.completed = 0;

    tasks[count++] = t;
    saveTasksToFile();

    printf("Task added.\n");
}


// Show all tasks
void displayTasks() {
    if (count == 0) {
        printf("No tasks.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("\nID: %d\n", tasks[i].id);
        printf("Title: %s\n", tasks[i].title);
        printf("Priority: %s\n", tasks[i].priority);
        printf("Deadline: %02d-%02d-%04d %02d:%02d\n",
               tasks[i].deadline.day,
               tasks[i].deadline.month,
               tasks[i].deadline.year,
               tasks[i].deadline.hour,
               tasks[i].deadline.minute);
        printf("Status: %s\n", tasks[i].completed ? "Completed" : "Pending");
    }
}


// Show pending or completed
void displayFiltered(int status) {
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (tasks[i].completed == status) {
            found = 1;
            printf("\nID: %d | %s | %s | %02d-%02d-%04d %02d:%02d\n",
                   tasks[i].id,
                   tasks[i].title,
                   tasks[i].priority,
                   tasks[i].deadline.day,
                   tasks[i].deadline.month,
                   tasks[i].deadline.year,
                   tasks[i].deadline.hour,
                   tasks[i].deadline.minute
            );
        }
    }

    if (!found)
        printf("No tasks found.\n");
}


// Update task info
void updateTask() {
    int id;
    printf("Enter ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {

            printf("New Title: ");
            scanf(" %[^\n]", tasks[i].title);

            printf("New Priority: ");
            scanf("%s", tasks[i].priority);

            printf("New Deadline (DD MM YYYY HH MM): ");
            scanf("%d %d %d %d %d",
                  &tasks[i].deadline.day,
                  &tasks[i].deadline.month,
                  &tasks[i].deadline.year,
                  &tasks[i].deadline.hour,
                  &tasks[i].deadline.minute);

            saveTasksToFile();
            printf("Updated.\n");
            return;
        }
    }

    printf("ID not found.\n");
}


// To mark task as completed
void markCompleted() {
    int id;
    printf("Enter ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            tasks[i].completed = 1;
            saveTasksToFile();
            printf("Done.\n");
            return;
        }
    }

    printf("Not found.\n");
}


// To delete task
void deleteTask() {
    int id;
    printf("Enter ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {

            for (int j = i; j < count - 1; j++)
                tasks[j] = tasks[j + 1];

            count--;
            saveTasksToFile();
            printf("Deleted.\n");
            return;
        }
    }

    printf("Not found.\n");
}


// Menu
int main() {
    loadTasksFromFile();

    int choice;
    while (1) {
        printf("\n--- TO-DO MENU ---\n");
        printf("1. Add Task\n");
        printf("2. Show All\n");
        printf("3. Show Pending\n");
        printf("4. Show Completed\n");
        printf("5. Update\n");
        printf("6. Mark Completed\n");
        printf("7. Delete\n");
        printf("8. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTask(); break;
            case 2: displayTasks(); break;
            case 3: displayFiltered(0); break;
            case 4: displayFiltered(1); break;
            case 5: updateTask(); break;
            case 6: markCompleted(); break;
            case 7: deleteTask(); break;
            case 8:
                printf("Bye.\n");
                return 0;
            default:
                printf("Invalid.\n");
        }
    }
}


