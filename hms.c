#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define CLEAR_CMD "cls"
#define PAUSE_CMD "pause"
#else
#define CLEAR_CMD "clear"
#define PAUSE_CMD "" /* we'll implement portable pause() */
#endif

struct Patient
{
    int id;
    char name[100];
    int age;
    char gender[20];
};

/* Function declarations */
void savePatient(struct Patient p);
void showAllPatients();
void searchPatient();
void receptionPortal();
void patientPortal();
void viewPatientDetails();
void viewAppointments();
void viewLabReports();
void viewPrescriptions();
void doctorPortal();
void labPortal();
void adminPortal();
void contributionPortal();
void feedback();

void clearScreen()
{
    /* portable clear */
    system(CLEAR_CMD);
}

/* portable pause */
void pauseProg()
{
#ifdef _WIN32
    system(PAUSE_CMD);
#else
    printf("\nPress Enter to continue...");
    int c;
    /* consume leftover newline */
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
    getchar();
#endif
}

/* safe read line that trims trailing newline */
void read_line(char *buffer, size_t size)
{
    if (fgets(buffer, (int)size, stdin) == NULL)
    {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

/* helper to trim leading/trailing spaces (in place) */
void trim(char *s)
{
    /* trim leading */
    char *start = s;
    while (*start && (*start == ' ' || *start == '\t'))
        start++;
    if (start != s)
        memmove(s, start, strlen(start) + 1);
    /* trim trailing */
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t'))
    {
        s[len - 1] = '\0';
        len--;
    }
}

/* Write patient to patients.txt as pipe-separated fields:
   id|name|age|gender\n
*/
void savePatient(struct Patient p)
{
    FILE *file = fopen("patients.txt", "a");
    if (!file)
    {
        perror("Error opening patients.txt for append");
        return;
    }
    /* Replace any '|' characters in name/gender to avoid corrupting delimiter */
    for (char *q = p.name; *q; ++q)
        if (*q == '|')
            *q = ' ';
    for (char *q = p.gender; *q; ++q)
        if (*q == '|')
            *q = ' ';
    fprintf(file, "%d|%s|%d|%s\n", p.id, p.name, p.age, p.gender);
    fclose(file);
}

void showAllPatients()
{
    FILE *file = fopen("patients.txt", "r");
    if (!file)
    {
        printf("\nNo patient records found.\n");
        pauseProg();
        return;
    }

    char line[512];
    printf("\n==== Patient List ====\n\n");
    while (fgets(line, sizeof(line), file))
    {
        /* parse line: id|name|age|gender */
        char *tok;
        int id;
        char name[100] = {0};
        int age;
        char gender[20] = {0};

        /* get id */
        tok = strtok(line, "|");
        if (!tok)
            continue;
        id = atoi(tok);

        tok = strtok(NULL, "|");
        if (tok)
            strncpy(name, tok, sizeof(name) - 1);

        tok = strtok(NULL, "|");
        if (tok)
            age = atoi(tok);
        else
            age = 0;

        tok = strtok(NULL, "|\n");
        if (tok)
            strncpy(gender, tok, sizeof(gender) - 1);

        trim(name);
        trim(gender);

        printf("ID: %d\nName: %s\nAge: %d\nGender: %s\n--------------\n",
               id, name, age, gender);
    }

    fclose(file);
    pauseProg();
}

void searchPatient()
{
    FILE *file = fopen("patients.txt", "r");
    if (!file)
    {
        printf("\nNo patient records found.\n");
        pauseProg();
        return;
    }

    int idToFind;
    printf("\nEnter Patient ID to search: ");
    if (scanf("%d", &idToFind) != 1)
    {
        while (getchar() != '\n')
        {
        }
        printf("Invalid input.\n");
        fclose(file);
        pauseProg();
        return;
    }
    while (getchar() != '\n')
    {
    } /* clear newline */

    char line[512];
    int found = 0;
    while (fgets(line, sizeof(line), file))
    {
        char *tok;
        int id;
        char name[100] = {0};
        int age;
        char gender[20] = {0};

        tok = strtok(line, "|");
        if (!tok)
            continue;
        id = atoi(tok);

        if (id != idToFind)
            continue;

        tok = strtok(NULL, "|");
        if (tok)
            strncpy(name, tok, sizeof(name) - 1);

        tok = strtok(NULL, "|");
        if (tok)
            age = atoi(tok);
        else
            age = 0;

        tok = strtok(NULL, "|\n");
        if (tok)
            strncpy(gender, tok, sizeof(gender) - 1);

        trim(name);
        trim(gender);

        printf("\nPatient Found!\n");
        printf("ID: %d\nName: %s\nAge: %d\nGender: %s\n", id, name, age, gender);
        found = 1;
        break;
    }

    if (!found)
        printf("\nPatient Not Found!\n");

    fclose(file);
    pauseProg();
}

void receptionPortal()
{
    int choice;

    while (1)
    {
        clearScreen();
        printf("\n========== RECEPTION PORTAL ==========\n");
        printf("1. Register New Patient\n");
        printf("2. Show All Registered Patients\n");
        printf("3. Search Patient by ID\n");
        printf("4. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
            {
            }
            choice = 0;
        }
        while (getchar() != '\n')
        {
        } /* consume newline */

        switch (choice)
        {
        case 1:
        {
            struct Patient p;
            char buffer[200];

            printf("\nEnter Patient ID: ");
            if (scanf("%d", &p.id) != 1)
            {
                while (getchar() != '\n')
                    ;
                printf("Invalid ID.\n");
                pauseProg();
                break;
            }
            while (getchar() != '\n')
            {
            }

            printf("Enter Patient Name: ");
            read_line(p.name, sizeof(p.name));

            printf("Enter Age: ");
            if (scanf("%d", &p.age) != 1)
            {
                while (getchar() != '\n')
                    ;
                printf("Invalid Age.\n");
                pauseProg();
                break;
            }
            while (getchar() != '\n')
            {
            }

            printf("Enter Gender: ");
            read_line(p.gender, sizeof(p.gender));

            trim(p.name);
            trim(p.gender);

            savePatient(p);
            printf("\nPatient Registered Successfully!\n");
            pauseProg();
            break;
        }
        case 2:
            showAllPatients();
            break;

        case 3:
            searchPatient();
            break;

        case 4:
            return;

        default:
            printf("\nInvalid choice!\n");
            pauseProg();
        }
    }
}

void viewPatientDetails()
{
    FILE *file = fopen("patients.txt", "r");
    if (!file)
    {
        printf("\nNo patient records found.\n");
        pauseProg();
        return;
    }

    int id;
    printf("\nEnter your Patient ID: ");
    if (scanf("%d", &id) != 1)
    {
        while (getchar() != '\n')
            ;
        printf("Invalid input.\n");
        fclose(file);
        pauseProg();
        return;
    }
    while (getchar() != '\n')
    {
    }

    char line[512];
    int found = 0;
    while (fgets(line, sizeof(line), file))
    {
        char *tok;
        int pid;
        char name[100] = {0};
        int age;
        char gender[20] = {0};

        tok = strtok(line, "|");
        if (!tok)
            continue;
        pid = atoi(tok);
        if (pid != id)
            continue;

        tok = strtok(NULL, "|");
        if (tok)
            strncpy(name, tok, sizeof(name) - 1);

        tok = strtok(NULL, "|");
        if (tok)
            age = atoi(tok);
        else
            age = 0;

        tok = strtok(NULL, "|\n");
        if (tok)
            strncpy(gender, tok, sizeof(gender) - 1);

        trim(name);
        trim(gender);

        printf("\n====== Your Details ======\n");
        printf("ID     : %d\n", pid);
        printf("Name   : %s\n", name);
        printf("Age    : %d\n", age);
        printf("Gender : %s\n", gender);
        found = 1;
        break;
    }

    if (!found)
        printf("\nNo record found for this ID.\n");

    fclose(file);
    pauseProg();
}

/* For appointments, labreports, prescriptions we expect pipe-separated formats:
   appointments.txt: pid|date|time|doctor
   labreports.txt: pid|report text...
   prescriptions.txt: pid|prescription text...
   We'll read lines and display those matching patient ID.
*/

void viewAppointments()
{
    FILE *file = fopen("appointments.txt", "r");
    if (!file)
    {
        printf("\nNo appointments found.\n");
        pauseProg();
        return;
    }

    int pid;
    printf("\nEnter your Patient ID: ");
    if (scanf("%d", &pid) != 1)
    {
        while (getchar() != '\n')
            ;
        printf("Invalid input.\n");
        fclose(file);
        pauseProg();
        return;
    }
    while (getchar() != '\n')
    {
    }

    char line[512];
    int found = 0;
    printf("\n====== Your Appointments ======\n");
    while (fgets(line, sizeof(line), file))
    {
        char *tok;
        int id;
        char date[64] = {0}, time[64] = {0}, doctor[128] = {0};

        tok = strtok(line, "|");
        if (!tok)
            continue;
        id = atoi(tok);
        if (id != pid)
            continue;

        tok = strtok(NULL, "|");
        if (tok)
            strncpy(date, tok, sizeof(date) - 1);

        tok = strtok(NULL, "|");
        if (tok)
            strncpy(time, tok, sizeof(time) - 1);

        tok = strtok(NULL, "|\n");
        if (tok)
            strncpy(doctor, tok, sizeof(doctor) - 1);

        trim(date);
        trim(time);
        trim(doctor);

        printf("Date   : %s\n", date);
        printf("Time   : %s\n", time);
        printf("Doctor : %s\n", doctor);
        printf("-----------------------------\n");
        found = 1;
    }

    if (!found)
        printf("No appointment booked.\n");

    fclose(file);
    pauseProg();
}

void viewLabReports()
{
    FILE *file = fopen("labreports.txt", "r");
    if (!file)
    {
        printf("\nNo lab reports found.\n");
        pauseProg();
        return;
    }

    int pid;
    printf("\nEnter your Patient ID: ");
    if (scanf("%d", &pid) != 1)
    {
        while (getchar() != '\n')
            ;
        printf("Invalid input.\n");
        fclose(file);
        pauseProg();
        return;
    }
    while (getchar() != '\n')
    {
    }

    char line[1024];
    int found = 0;
    printf("\n====== Your Lab Reports ======\n");
    while (fgets(line, sizeof(line), file))
    {
        char *tok;
        int id;
        char report[900] = {0};

        tok = strtok(line, "|");
        if (!tok)
            continue;
        id = atoi(tok);
        if (id != pid)
            continue;

        tok = strtok(NULL, "|\n");
        if (tok)
            strncpy(report, tok, sizeof(report) - 1);
        trim(report);

        printf("Report: %s\n", report);
        printf("-----------------------------\n");
        found = 1;
    }

    if (!found)
        printf("No lab report available.\n");

    fclose(file);
    pauseProg();
}

void viewPrescriptions()
{
    FILE *file = fopen("prescriptions.txt", "r");
    if (!file)
    {
        printf("\nNo prescriptions found.\n");
        pauseProg();
        return;
    }

    int pid;
    printf("\nEnter your Patient ID: ");
    if (scanf("%d", &pid) != 1)
    {
        while (getchar() != '\n')
            ;
        printf("Invalid input.\n");
        fclose(file);
        pauseProg();
        return;
    }
    while (getchar() != '\n')
    {
    }

    char line[1024];
    int found = 0;
    printf("\n====== Your Prescriptions ======\n");
    while (fgets(line, sizeof(line), file))
    {
        char *tok;
        int id;
        char pres[900] = {0};

        tok = strtok(line, "|");
        if (!tok)
            continue;
        id = atoi(tok);
        if (id != pid)
            continue;

        tok = strtok(NULL, "|\n");
        if (tok)
            strncpy(pres, tok, sizeof(pres) - 1);
        trim(pres);

        printf("Prescription: %s\n", pres);
        printf("-----------------------------\n");
        found = 1;
    }

    if (!found)
        printf("No prescription available.\n");

    fclose(file);
    pauseProg();
}

void patientPortal()
{
    int choice;
    while (1)
    {
        clearScreen();
        printf("\n=============== PATIENT PORTAL ===============\n");
        printf("1. View My Details\n");
        printf("2. View Appointments\n");
        printf("3. View Lab Reports\n");
        printf("4. View Prescriptions\n");
        printf("5. Back to Main Menu\n");

        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            choice = 0;
        }
        while (getchar() != '\n')
        {
        }

        switch (choice)
        {
        case 1:
            viewPatientDetails();
            break;
        case 2:
            viewAppointments();
            break;
        case 3:
            viewLabReports();
            break;
        case 4:
            viewPrescriptions();
            break;
        case 5:
            return;
        default:
            printf("\nInvalid Input!\n");
            pauseProg();
        }
    }
}

void doctorPortal()
{
    clearScreen();
    printf("\n---Welcome To Doctor Portal ---\n");
    printf("1. View patient list (use Reception -> Show All Registered Patients)\n");
    printf("2. Write Prescription (not implemented)\n");
    printf("3. Update medical record (not implemented)\n");
    printf("More features can be added...\n");
    pauseProg();
}

void labPortal()
{
    clearScreen();
    printf("\n---Welcome To Lab Portal ---\n");
    printf("1. Test report entry (not implemented)\n");
    printf("2. Print report (not implemented)\n");
    printf("More features can be added...\n");
    pauseProg();
}

void adminPortal()
{
    clearScreen();
    printf("\n---Welcome To Admin Portal ---\n");
    printf("1. Add staff (not implemented)\n");
    printf("2. Remove staff (not implemented)\n");
    printf("3. Edit database (not implemented)\n");
    printf("More features can be added...\n");
    pauseProg();
}

void contributionPortal()
{
    clearScreen();
    printf("\n--- Team Contribution & Feedback ---\n");
    printf("Team Name: UPSIDEDOWN\n\n");
    printf("1. Project Concept: Hospital Management System\n");
    printf("2. Team Roles:\n");
    printf("   - Member 1: Coding\n");
    printf("   - Member 2: Documentation\n");
    printf("   - Member 3: UI Flow / Presentation\n");
    printf("\nFeedback: This project is built for educational purposes.\n");
    pauseProg();
}

/* Collect feedback from user and append to feedback.txt */
void feedback()
{
    clearScreen();
    int id;
    char fb[1000];

    printf("Enter your Patient ID (or 0 to remain anonymous): ");
    if (scanf("%d", &id) != 1)
    {
        while (getchar() != '\n')
            ;
        id = 0;
    }
    while (getchar() != '\n')
    {
    }

    printf("Enter your feedback (single line):\n");
    read_line(fb, sizeof(fb));
    trim(fb);

    FILE *f = fopen("feedback.txt", "a");
    if (!f)
    {
        perror("Unable to open feedback.txt");
        pauseProg();
        return;
    }
    /* store as: id|feedback\n */
    fprintf(f, "%d|%s\n", id, fb);
    fclose(f);

    printf("\nThank you for your feedback!\n");
    pauseProg();
}

int main()
{
    int choice;

    while (1)
    {
        clearScreen();
        printf("\n=====================================================\n");
        printf("         WELCOME TO HOSPITAL MANAGEMENT SYSTEM\n");
        printf("                     TEAM: UPSIDEDOWN\n");
        printf("=====================================================\n");

        printf("\nChoose a Portal:\n");
        printf("1. Reception\n");
        printf("2. Patient\n");
        printf("3. Doctor\n");
        printf("4. Lab\n");
        printf("5. Admin\n");
        printf("6. Team Details\n");
        printf("7. Feedback\n");
        printf("8. Exit\n");

        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
            {
            }
            choice = 0;
        }
        while (getchar() != '\n')
        {
        }

        switch (choice)
        {
        case 1:
            receptionPortal();
            break;
        case 2:
            patientPortal();
            break;
        case 3:
            doctorPortal();
            break;
        case 4:
            labPortal();
            break;
        case 5:
            adminPortal();
            break;
        case 6:
            contributionPortal();
            break;
        case 7:
            feedback();
            break;
        case 8:
            printf("\nThank you for using the system!\n");
            exit(0);
        default:
            printf("\nInvalid choice! Try again.\n");
            pauseProg();
            break;
        }
    }

    return 0;
}
