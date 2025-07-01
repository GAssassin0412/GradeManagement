#include <stdio.h>

// Ĭ�ϵ�¼��Ϣ
// ��ʦ: admin / password
// ѧ��: ѧ�� / ѧ��

// --- �������� ---
#define MAX_STUDENTS 100      // ���ѧ������
#define MAX_TEACHERS 10       // ����ʦ����
#define FILENAME "grades.txt" // ѧ�������ļ���
#define TEACHER_FILENAME "TeacherList.txt" // ��ʦ�����ļ���

// ѧ�ƶ���
#define MAX_SUBJECTS 3
const char* SUBJECT_NAMES[MAX_SUBJECTS] = {"����", "��ѧ", "Ӣ��"};


// --- ���ݽṹ ---
// ѧ����Ϣ�ṹ��
struct Student {
    long long id;
    char name[20];
    float scores[MAX_SUBJECTS]; // ���Ƴɼ�
    char password[20];          // ���� (�Զ���Ϊѧ��)
};

// ��ʦ��Ϣ�ṹ��
struct Teacher {
    char username[20];
    char password[20];
};

// --- ȫ�ֱ��� ---
// ���ڴ洢����ѧ���ͽ�ʦ��Ϣ
struct Student students[MAX_STUDENTS];
int studentCount = 0;

struct Teacher teachers[MAX_TEACHERS];
int teacherCount = 0;

// --- ���ߺ��� ---
// �Զ����ַ����ȽϺ��� (��� strcmp)
int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// --- �������� ---
void login();
void menu();
void studentMenu(int studentIndex);
void clearInputBuffer();
void manageStudents();
void addStudent();
void deleteStudent();
void modifyStudent();
void searchStudent();
int findStudentById(long long id);
void listStudents();
void saveData();
void loadData();
void loadTeachers();
void rankMenu();
void rankByAverage();
void rankBySubject();
void displayStudentList(const struct Student list[], int count, const char* title);
float calculateAverage(const struct Student* s);
void swapStudents(struct Student* a, struct Student* b);


// ������ - �������
int main() {
    loadData();     // ���ļ�����ѧ������
    loadTeachers(); // ���ļ����ؽ�ʦ����

    printf("��ӭʹ��ѧ���ɼ�����ϵͳ��\n");
    printf("���س��������¼����...\n");
    clearInputBuffer();

    login(); // �����¼����

    return 0;
}

// --- ���Ĺ���ʵ�� ---

// ��¼����
void login() {
    char username[20];
    char password[20];
    int loggedIn = 0;

    while (!loggedIn) {
        printf("\n--- ϵͳ��¼ ---\n");
        printf("�������û���: ");
        scanf("%19s", username);
        clearInputBuffer();

        printf("����������: ");
        scanf("%19s", password);
        clearInputBuffer();

        // ����Ƿ�Ϊ��ʦ
        int isTeacher = 0;
        if (teacherCount > 0) { // ���ȴ��ļ���֤
            for (int i = 0; i < teacherCount; i++) {
                if (my_strcmp(teachers[i].username, username) == 0 && my_strcmp(teachers[i].password, password) == 0) {
                    isTeacher = 1;
                    break;
                }
            }
        } else { // ���ļ������ڣ�ʹ��Ĭ���˺�
            if (my_strcmp(username, "admin") == 0 && my_strcmp(password, "password") == 0) {
                isTeacher = 1;
            }
        }

        if (isTeacher) {
            printf("\n��ʦ %s ��¼�ɹ���\n", username);
            loggedIn = 1;
            menu(); // �����ʦ�˵�
            continue;
        }

        // ����Ƿ�Ϊѧ�� (ѧ�ź�������ͬ)
        long long studentId;
        int studentIndex = -1;
        if (my_strcmp(username, password) == 0) {
            sscanf(username, "%lld", &studentId);
            studentIndex = findStudentById(studentId);

            if (studentIndex != -1) {
                printf("\nѧ�� %s ��¼�ɹ���\n", students[studentIndex].name);
                loggedIn = 1;
                studentMenu(studentIndex); // ����ѧ���˵�
                continue;
            }
        }

        // ��¼ʧ��
        printf("�û�����������������ԡ�\n");
    }
}

// �������뻺����
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ��ʦ���˵�
void menu() {
    int choice;
    do {
        printf("\n----- ѧ���ɼ�����ϵͳ (��ʦģʽ) -----\n");
        printf("1. ѧ������ (���/ɾ��/�޸�/��ѯ)\n");
        printf("2. ��ʾ����ѧ��\n");
        printf("3. �ɼ�����\n");
        printf("4. �������ݵ��ļ�\n");
        printf("0. �˳���¼\n");
        printf("--------------------------------------\n");
        printf("����������ѡ��: ");

        while (scanf("%d", &choice) != 1) {
            printf("������Ч��������һ������: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        switch (choice) {
            case 1: manageStudents(); break;
            case 2: listStudents();   break;
            case 3: rankMenu();       break;
            case 4: saveData();       break;
            case 0: printf("\n�ѳɹ��˳���¼��\n"); break;
            default: printf("��Ч��ѡ�����������롣\n"); break;
        }

        if (choice != 0) {
            printf("\n���س�������...\n");
            clearInputBuffer();
        }
    } while (choice != 0);
}

// ѧ�����˲˵�
void studentMenu(int studentIndex) {
    int choice;
    do {
        printf("\n----- ѧ���������� -----\n");
        printf("��ӭ��, %s ͬѧ��\n", students[studentIndex].name);
        printf("1. �鿴�ҵĳɼ�\n");
        printf("0. �˳���¼\n");
        printf("------------------------\n");
        printf("����������ѡ��: ");

        while (scanf("%d", &choice) != 1) {
            printf("������Ч��������һ������: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        switch (choice) {
            case 1: // �鿴���˳ɼ�
                printf("\n--- ��ĳɼ���Ϣ ---\n");
                printf("%-10s: %lld\n", "ѧ��", students[studentIndex].id);
                printf("%-10s: %s\n", "����", students[studentIndex].name);
                printf("���Ƴɼ�����:\n");
                for (int i = 0; i < MAX_SUBJECTS; i++) {
                    printf("  - %-8s: %.2f\n", SUBJECT_NAMES[i], students[studentIndex].scores[i]);
                }
                float average_score = calculateAverage(&students[studentIndex]);
                printf("------------------------\n");
                printf("%-10s: %.2f\n", "ƽ����", average_score);
                break;
            case 0:
                printf("\n�ѳɹ��˳���¼��\n");
                break;
            default:
                printf("��Ч��ѡ�����������롣\n");
                break;
        }
        if (choice != 0) {
            printf("\n���س�������...\n");
            clearInputBuffer();
        }
    } while (choice != 0);
}

// ѧ�������Ӳ˵�
void manageStudents() {
    int subChoice;
    do {
        printf("\n----- ѧ�������Ӳ˵� -----\n");
        printf("1. ���ѧ��\n");
        printf("2. ɾ��ѧ��\n");
        printf("3. �޸�ѧ��\n");
        printf("4. ��ѯѧ��\n");
        printf("0. �������˵�\n");
        printf("-------------------------\n");
        printf("����������ѡ��: ");

        while (scanf("%d", &subChoice) != 1) {
            printf("������Ч��������һ������: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        switch (subChoice) {
            case 1: addStudent();    break;
            case 2: deleteStudent(); break;
            case 3: modifyStudent(); break;
            case 4: searchStudent(); break;
            case 0: printf("�������˵���\n"); break;
            default: printf("��Ч��ѡ�����������롣\n"); break;
        }

        if (subChoice != 0) {
            printf("\n���س�������...\n");
            clearInputBuffer();
        }
    } while (subChoice != 0);
}

// �����ѧ��
void addStudent() {
    printf("\n--- ���ѧ�� ---\n");
    if (studentCount >= MAX_STUDENTS) {
        printf("ѧ�������Ѵﵽ���ޡ�\n");
        return;
    }
    int i = studentCount;
    printf("������ѧ��: ");
    while (scanf("%lld", &students[i].id) != 1) {
        printf("������Ч��������һ��������Ϊѧ��: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    // ���ѧ���Ƿ��Ѵ���
    if (findStudentById(students[i].id) != -1) {
        printf("����ѧ�� %lld �Ѵ��ڣ���ʹ������ѧ�š�\n", students[i].id);
        return;
    }
    sprintf(students[i].password, "%lld", students[i].id); // Ĭ��������ѧ����ͬ
    printf("���������� (�޿ո�): ");
    scanf("%19s", students[i].name);
    clearInputBuffer();
    printf("������������Ƴɼ� (0-100):\n");
    for (int j = 0; j < MAX_SUBJECTS; j++) {
        printf("������ %s �ɼ�: ", SUBJECT_NAMES[j]);
        while (scanf("%f", &students[i].scores[j]) != 1 ||
               students[i].scores[j] < 0 || students[i].scores[j] > 100) {
            printf("������Ч��������һ��0��100֮�������: ");
            clearInputBuffer();
        }
        clearInputBuffer();
    }
    studentCount++;
    printf("ѧ����ӳɹ���Ĭ��������ѧ����ͬ��\n");
}

// ����ѧ�Ų���ѧ��������������δ�ҵ�����-1
int findStudentById(long long id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            return i;
        }
    }
    return -1;
}

// ɾ��ָ��ѧ�ŵ�ѧ��
void deleteStudent() {
    printf("\n--- ɾ��ѧ�� ---\n");
    if (studentCount == 0) {
        printf("��ǰû��ѧ����Ϣ��ɾ����\n");
        return;
    }
    long long id_to_delete;
    printf("������Ҫɾ����ѧ����ѧ��: ");
    while (scanf("%lld", &id_to_delete) != 1) {
        printf("������Ч��������һ��������Ϊѧ��: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    int index = findStudentById(id_to_delete);
    if (index == -1) {
        printf("δ�ҵ�ѧ��Ϊ %lld ��ѧ����\n", id_to_delete);
        return;
    }
    // ������Ԫ��ǰ�ƣ�����Ҫɾ����Ԫ��
    for (int i = index; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }
    studentCount--;
    printf("ѧ��Ϊ %lld ��ѧ���ѳɹ�ɾ����\n", id_to_delete);
}

// �޸�ָ��ѧ�ŵ�ѧ����Ϣ
void modifyStudent() {
    printf("\n--- �޸�ѧ����Ϣ ---\n");
    if (studentCount == 0) {
        printf("��ǰû��ѧ����Ϣ���޸ġ�\n");
        return;
    }
    long long id_to_modify;
    printf("������Ҫ�޸ĵ�ѧ����ѧ��: ");
    while (scanf("%lld", &id_to_modify) != 1) {
        printf("������Ч��������һ��������Ϊѧ��: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    int index = findStudentById(id_to_modify);
    if (index == -1) {
        printf("δ�ҵ�ѧ��Ϊ %lld ��ѧ����\n", id_to_modify);
        return;
    }
    printf("�ҵ���ѧ������ǰ��Ϣ���£�\n");
    printf("%-10s: %lld\n", "ѧ��", students[index].id);
    printf("%-10s: %s\n", "����", students[index].name);
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("  - %-8s: %.2f\n", SUBJECT_NAMES[i], students[index].scores[i]);
    }
    printf("\n�뿪ʼ��������Ϣ (���뽫�Զ�����Ϊ��ѧ��)��\n");
    printf("�������µ�ѧ��: ");
    long long new_id;
    while (scanf("%lld", &new_id) != 1) {
        printf("������Ч��������һ��������Ϊѧ��: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    // �����ѧ���Ƿ��ͻ
    if (new_id != students[index].id && findStudentById(new_id) != -1) {
        printf("������ѧ�� %lld �ѱ�����ѧ��ʹ�ã��޸�ʧ�ܡ�\n", new_id);
        return;
    }
    students[index].id = new_id;
    sprintf(students[index].password, "%lld", students[index].id); // ��������
    printf("�������µ����� (�޿ո�): ");
    scanf("%19s", students[index].name);
    clearInputBuffer();
    printf("�����������µĸ��Ƴɼ� (0-100):\n");
    for (int j = 0; j < MAX_SUBJECTS; j++) {
        printf("�������µ� %s �ɼ�: ", SUBJECT_NAMES[j]);
        while (scanf("%f", &students[index].scores[j]) != 1 ||
               students[index].scores[j] < 0 || students[index].scores[j] > 100) {
            printf("������Ч��������һ��0��100֮�������: ");
            clearInputBuffer();
        }
        clearInputBuffer();
    }
    printf("ѧ����Ϣ�޸ĳɹ���\n");
}

// ��ѯָ��ѧ�ŵ�ѧ����Ϣ
void searchStudent() {
    printf("\n--- ��ѯѧ����Ϣ ---\n");
    if (studentCount == 0) {
        printf("��ǰû��ѧ����Ϣ�ɲ�ѯ��\n");
        return;
    }
    long long id_to_search;
    printf("������Ҫ��ѯ��ѧ����ѧ��: ");
    while (scanf("%lld", &id_to_search) != 1) {
        printf("������Ч��������һ��������Ϊѧ��: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    int index = findStudentById(id_to_search);
    if (index == -1) {
        printf("δ�ҵ�ѧ��Ϊ %lld ��ѧ����\n", id_to_search);
    } else {
        printf("�ҵ���ѧ������Ϣ���£�\n");
        printf("%-10s: %lld\n", "ѧ��", students[index].id);
        printf("%-10s: %s\n", "����", students[index].name);
        printf("���Ƴɼ�:\n");
        for (int i = 0; i < MAX_SUBJECTS; i++) {
             printf("  - %-8s: %.2f\n", SUBJECT_NAMES[i], students[index].scores[i]);
        }
        float average_score = calculateAverage(&students[index]);
        printf("------------------------\n");
        printf("%-10s: %.2f\n", "ƽ����", average_score);
    }
}

// ��Ĭ��˳����ʾ����ѧ��
void listStudents() {
    displayStudentList(students, studentCount, "--- ѧ����Ϣ�б� (�����) ---");
}

// ���������Ӳ˵�
void rankMenu() {
    int choice;
    do {
        printf("\n----- �ɼ������Ӳ˵� -----\n");
        printf("1. ��������� (Ĭ��)\n");
        printf("2. ��ƽ��������\n");
        printf("3. �����Ƴɼ�����\n");
        printf("0. �������˵�\n");
        printf("---------------------------\n");
        printf("����������ѡ��: ");

        while (scanf("%d", &choice) != 1) {
            printf("������Ч��������һ������: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        switch (choice) {
            case 1: listStudents();    break;
            case 2: rankByAverage();   break;
            case 3: rankBySubject();   break;
            case 0: printf("�������˵���\n"); break;
            default: printf("��Ч��ѡ�����������롣\n"); break;
        }

        if (choice != 0) {
            printf("\n���س�������...\n");
            clearInputBuffer();
        }
    } while (choice != 0);
}

// ��ƽ���ֶ�ѧ��������������ʾ
void rankByAverage() {
    if (studentCount == 0) {
        printf("��ǰû��ѧ����Ϣ��������\n");
        return;
    }

    // ������ʱ���飬�����޸�ԭʼ����˳��
    struct Student tempStudents[MAX_STUDENTS];
    for (int i = 0; i < studentCount; i++) {
        tempStudents[i] = students[i];
    }

    // ð������ (��ƽ���ֽ���)
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            if (calculateAverage(&tempStudents[j]) < calculateAverage(&tempStudents[j+1])) {
                swapStudents(&tempStudents[j], &tempStudents[j+1]);
            }
        }
    }

    // ��ʾ������
    displayStudentList(tempStudents, studentCount, "--- ѧ���ɼ��б� (��ƽ��������) ---");
}

// ��ָ�����Ƴɼ���ѧ��������������ʾ
void rankBySubject() {
    if (studentCount == 0) {
        printf("��ǰû��ѧ����Ϣ��������\n");
        return;
    }

    // �û�ѡ�������Ŀ�Ŀ
    int subject_choice;
    printf("\n��ѡ��Ҫ�����Ŀ�Ŀ:\n");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("%d. %s\n", i + 1, SUBJECT_NAMES[i]);
    }
    printf("�������Ŀ���: ");
    while (scanf("%d", &subject_choice) != 1 || subject_choice < 1 || subject_choice > MAX_SUBJECTS) {
        printf("������Ч�������� 1 �� %d ֮�������: ", MAX_SUBJECTS);
        clearInputBuffer();
    }
    clearInputBuffer();
    int subject_index = subject_choice - 1;

    // ������ʱ����
    struct Student tempStudents[MAX_STUDENTS];
    for (int i = 0; i < studentCount; i++) {
        tempStudents[i] = students[i];
    }

    // ð������ (��ָ����Ŀ�ɼ�����)
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            if (tempStudents[j].scores[subject_index] < tempStudents[j+1].scores[subject_index]) {
                swapStudents(&tempStudents[j], &tempStudents[j+1]);
            }
        }
    }

    // ��ʾ������
    char title[100];
    sprintf(title, "--- ѧ���ɼ��б� (��%s����) ---", SUBJECT_NAMES[subject_index]);
    displayStudentList(tempStudents, studentCount, title);
}

// ͨ�ú�������ʾѧ���б� (����ͷ������)
void displayStudentList(const struct Student list[], int count, const char* title) {
    printf("\n%s\n", title);
    if (count == 0) {
        printf("��ǰû��ѧ����Ϣ����ʾ��\n");
        return;
    }

    // ��ӡ��ͷ
    printf("%-4s%-16s%-12s", "����", "ѧ��", "����");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("%-10s", SUBJECT_NAMES[i]);
    }
    printf("%-10s\n", "ƽ����");
    printf("----------------------------------------------------------------------------\n");

    // ��ӡѧ����Ϣ
    for (int i = 0; i < count; i++) {
        printf("%-4d%-16lld%-12s", i + 1, list[i].id, list[i].name);

        for (int j = 0; j < MAX_SUBJECTS; j++) {
            printf("%-10.2f", list[i].scores[j]);
        }

        float average_score = calculateAverage(&list[i]);
        printf("%-10.2f\n", average_score);
    }

    printf("----------------------------------------------------------------------------\n");
    printf("�ܼ� %d ��ѧ����\n", count);
}


// --- �������� ---

// ���㵥��ѧ����ƽ����
float calculateAverage(const struct Student* s) {
    if (MAX_SUBJECTS == 0) return 0.0f;
    float total = 0;
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        total += s->scores[i];
    }
    return total / MAX_SUBJECTS;
}

// ��������ѧ���ṹ�������
void swapStudents(struct Student* a, struct Student* b) {
    struct Student temp = *a;
    *a = *b;
    *b = temp;
}


// --- �ļ����� ---

// ����ѧ�����ݵ��ļ�
void saveData() {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("�����޷����ļ� %s ����д�롣\n", FILENAME);
        return;
    }
    fprintf(fp, "%d\n", studentCount); // ����д��ѧ������
    for (int i = 0; i < studentCount; i++) {
        fprintf(fp, "%lld %s", students[i].id, students[i].name);
        for (int j = 0; j < MAX_SUBJECTS; j++) {
            fprintf(fp, " %.2f", students[i].scores[j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("�����ѳɹ����浽�ļ� %s��\n", FILENAME);
}

// ���ļ�����ѧ������
void loadData() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) { // �ļ������������������������
        studentCount = 0;
        return;
    }
    int fileStudentCount = 0;
    if (fscanf(fp, "%d\n", &fileStudentCount) != 1) { // ��ȡѧ������
        studentCount = 0;
        fclose(fp);
        return;
    }
    if (fileStudentCount > MAX_STUDENTS) { // ��ֹ���
        fileStudentCount = MAX_STUDENTS;
    }
    studentCount = 0;
    for (int i = 0; i < fileStudentCount; i++) {
        // ��ȡһ��ѧ������
        if (fscanf(fp, "%lld %19s", &students[i].id, students[i].name) == 2) {
            int success = 1;
            for (int j = 0; j < MAX_SUBJECTS; j++) {
                if (fscanf(fp, "%f", &students[i].scores[j]) != 1) {
                    success = 0; // ����ɼ���ȡʧ�ܣ���Ǵ���������Ч
                    break;
                }
            }
            if (success) {
                sprintf(students[i].password, "%lld", students[i].id); // �Զ���������
                studentCount++;
            } else {
                printf("���棺�ļ� %s ��ʽ���󣬼��ز������ݡ�\n", FILENAME);
                break; // ֹͣ�����Է��������
            }
        } else {
            break; // �޷���ȡѧ�ź�������ֹͣ����
        }
    }
    fclose(fp);
}

// ���ļ����ؽ�ʦ����
void loadTeachers() {
    FILE *fp = fopen(TEACHER_FILENAME, "r");
    if (fp == NULL) {
        printf("��ʾ: δ�ҵ���ʦ�б��ļ� %s����ʹ��Ĭ�Ͻ�ʦ�˺� (admin/password)��\n", TEACHER_FILENAME);
        teacherCount = 0;
        return;
    }
    teacherCount = 0;
    // ѭ����ȡ��ʦ�˺����룬ֱ���ļ�ĩβ��ﵽ�������
    while (teacherCount < MAX_TEACHERS && fscanf(fp, "%s %s", teachers[teacherCount].username, teachers[teacherCount].password) == 2) {
        teacherCount++;
    }
    fclose(fp);
}