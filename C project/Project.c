#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// Structure to store task details
typedef struct {
    char title[100];
    char priority[10];
    char deadline[12];
    int completed; // 0 = pending, 1 = completed
} Task;

Task tasks[MAX];
int taskCount = 0;

// Function to load tasks from file
void loadTasks() {
    FILE *fp = fopen("tasks.txt", "r");
    if (!fp) return;

    taskCount = 0;
    while (fscanf(fp, "%[^;];%[^;];%[^;];%d\n",
                  tasks[taskCount].title,
                  tasks[taskCount].priority,
                  tasks[taskCount].deadline,
                  &tasks[taskCount].completed) == 4) {
        taskCount++;
    }
    fclose(fp);
}

// Save tasks to file
void saveTasks() {
    FILE *fp = fopen("tasks.txt", "w");
    for (int i = 0; i < taskCount; i++) {
        fprintf(fp, "%s;%s;%s;%d\n",
                tasks[i].title,
                tasks[i].priority,
                tasks[i].deadline,
                tasks[i].completed);
    }
    fclose(fp);
}

// Add a new task
void addTask() {
    if (taskCount >= MAX) {
        printf("Task limit reached!\n");
        return;
    }

    Task t;
    printf("Enter task title: ");
    getchar();  
    fgets(t.title, sizeof(t.title), stdin);
    t.title[strcspn(t.title, "\n")] = 0;

    printf("Priority (High/Medium/Low): ");
    scanf("%s", t.priority);

    printf("Deadline (DD/MM/YYYY): ");
    scanf("%s", t.deadline);

    t.completed = 0;

    tasks[taskCount++] = t;
    printf("Task added!\n");
    saveTasks();
}

// Display tasks
void viewTasks() {
    if (taskCount == 0) {
        printf("No tasks added yet.\n");
        return;
    }

    printf("\n--- TO-DO LIST ---\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. %s | %s | %s | %s\n",
               i + 1,
               tasks[i].title,
               tasks[i].priority,
               tasks[i].deadline,
               tasks[i].completed ? "Completed" : "Pending");
    }
}

// Mark task as completed
void markCompleted() {
    int n;
    viewTasks();
    printf("Enter task number to mark completed: ");
    scanf("%d", &n);

    if (n < 1 || n > taskCount) {
        printf("Invalid task number!\n");
        return;
    }

    tasks[n - 1].completed = 1;
    printf("Task marked completed!\n");
    saveTasks();
}

// Delete a task
void deleteTask() {
    int n;
    viewTasks();
    printf("Enter task number to delete: ");
    scanf("%d", &n);

    if (n < 1 || n > taskCount) {
        printf("Invalid task number!\n");
        return;
    }

    // shift all elements left
    for (int i = n - 1; i < taskCount - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    taskCount--;
    printf("Task deleted!\n");
    saveTasks();
}

// Main Menu
int main() {
    int choice;
    loadTasks();

    while (1) {
        printf("\n========== TO-DO LIST MENU ==========\n");
        printf("1. Add Task\n");
        printf("2. View All Tasks\n");
        printf("3. Mark Task Completed\n");
        printf("4. Delete Task\n");
        printf("5. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: markCompleted(); break;
            case 4: deleteTask(); break;
            case 5: saveTasks(); printf("Goodbye!\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}
