#include <stdio.h>

// 默认登录信息
// 教师: admin / password
// 学生: 学号 / 学号

// --- 常量定义 ---
#define MAX_STUDENTS 100      // 最大学生数量
#define MAX_TEACHERS 10       // 最大教师数量
#define FILENAME "grades.txt" // 学生数据文件名
#define TEACHER_FILENAME "TeacherList.txt" // 教师数据文件名

// 学科定义
#define MAX_SUBJECTS 3
const char* SUBJECT_NAMES[MAX_SUBJECTS] = {"语文", "数学", "英语"};


// --- 数据结构 ---
// 学生信息结构体
struct Student {
    long long id;
    char name[20];
    float scores[MAX_SUBJECTS]; // 各科成绩
    char password[20];          // 密码 (自动设为学号)
};

// 教师信息结构体
struct Teacher {
    char username[20];
    char password[20];
};

// --- 全局变量 ---
// 用于存储所有学生和教师信息
struct Student students[MAX_STUDENTS];
int studentCount = 0;

struct Teacher teachers[MAX_TEACHERS];
int teacherCount = 0;

// --- 工具函数 ---
// 自定义字符串比较函数 (替代 strcmp)
int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// --- 函数声明 ---
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


// 主函数 - 程序入口
int main() {
    loadData();     // 从文件加载学生数据
    loadTeachers(); // 从文件加载教师数据

    printf("欢迎使用学生成绩管理系统！\n");
    printf("按回车键进入登录界面...\n");
    clearInputBuffer();

    login(); // 进入登录流程

    return 0;
}

// --- 核心功能实现 ---

// 登录处理
void login() {
    char username[20];
    char password[20];
    int loggedIn = 0;

    while (!loggedIn) {
        printf("\n--- 系统登录 ---\n");
        printf("请输入用户名: ");
        scanf("%19s", username);
        clearInputBuffer();

        printf("请输入密码: ");
        scanf("%19s", password);
        clearInputBuffer();

        // 检查是否为教师
        int isTeacher = 0;
        if (teacherCount > 0) { // 优先从文件验证
            for (int i = 0; i < teacherCount; i++) {
                if (my_strcmp(teachers[i].username, username) == 0 && my_strcmp(teachers[i].password, password) == 0) {
                    isTeacher = 1;
                    break;
                }
            }
        } else { // 若文件不存在，使用默认账号
            if (my_strcmp(username, "admin") == 0 && my_strcmp(password, "password") == 0) {
                isTeacher = 1;
            }
        }

        if (isTeacher) {
            printf("\n教师 %s 登录成功！\n", username);
            loggedIn = 1;
            menu(); // 进入教师菜单
            continue;
        }

        // 检查是否为学生 (学号和密码相同)
        long long studentId;
        int studentIndex = -1;
        if (my_strcmp(username, password) == 0) {
            sscanf(username, "%lld", &studentId);
            studentIndex = findStudentById(studentId);

            if (studentIndex != -1) {
                printf("\n学生 %s 登录成功！\n", students[studentIndex].name);
                loggedIn = 1;
                studentMenu(studentIndex); // 进入学生菜单
                continue;
            }
        }

        // 登录失败
        printf("用户名或密码错误，请重试。\n");
    }
}

// 清理输入缓冲区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 教师主菜单
void menu() {
    int choice;
    do {
        printf("\n----- 学生成绩管理系统 (教师模式) -----\n");
        printf("1. 学生管理 (添加/删除/修改/查询)\n");
        printf("2. 显示所有学生\n");
        printf("3. 成绩排名\n");
        printf("4. 保存数据到文件\n");
        printf("0. 退出登录\n");
        printf("--------------------------------------\n");
        printf("请输入您的选择: ");

        while (scanf("%d", &choice) != 1) {
            printf("输入无效，请输入一个数字: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        switch (choice) {
            case 1: manageStudents(); break;
            case 2: listStudents();   break;
            case 3: rankMenu();       break;
            case 4: saveData();       break;
            case 0: printf("\n已成功退出登录。\n"); break;
            default: printf("无效的选择，请重新输入。\n"); break;
        }

        if (choice != 0) {
            printf("\n按回车键继续...\n");
            clearInputBuffer();
        }
    } while (choice != 0);
}

// 学生个人菜单
void studentMenu(int studentIndex) {
    int choice;
    do {
        printf("\n----- 学生个人中心 -----\n");
        printf("欢迎你, %s 同学！\n", students[studentIndex].name);
        printf("1. 查看我的成绩\n");
        printf("0. 退出登录\n");
        printf("------------------------\n");
        printf("请输入您的选择: ");

        while (scanf("%d", &choice) != 1) {
            printf("输入无效，请输入一个数字: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        switch (choice) {
            case 1: // 查看个人成绩
                printf("\n--- 你的成绩信息 ---\n");
                printf("%-10s: %lld\n", "学号", students[studentIndex].id);
                printf("%-10s: %s\n", "姓名", students[studentIndex].name);
                printf("各科成绩详情:\n");
                for (int i = 0; i < MAX_SUBJECTS; i++) {
                    printf("  - %-8s: %.2f\n", SUBJECT_NAMES[i], students[studentIndex].scores[i]);
                }
                float average_score = calculateAverage(&students[studentIndex]);
                printf("------------------------\n");
                printf("%-10s: %.2f\n", "平均分", average_score);
                break;
            case 0:
                printf("\n已成功退出登录。\n");
                break;
            default:
                printf("无效的选择，请重新输入。\n");
                break;
        }
        if (choice != 0) {
            printf("\n按回车键继续...\n");
            clearInputBuffer();
        }
    } while (choice != 0);
}

// 学生管理子菜单
void manageStudents() {
    int subChoice;
    do {
        printf("\n----- 学生管理子菜单 -----\n");
        printf("1. 添加学生\n");
        printf("2. 删除学生\n");
        printf("3. 修改学生\n");
        printf("4. 查询学生\n");
        printf("0. 返回主菜单\n");
        printf("-------------------------\n");
        printf("请输入您的选择: ");

        while (scanf("%d", &subChoice) != 1) {
            printf("输入无效，请输入一个数字: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        switch (subChoice) {
            case 1: addStudent();    break;
            case 2: deleteStudent(); break;
            case 3: modifyStudent(); break;
            case 4: searchStudent(); break;
            case 0: printf("返回主菜单。\n"); break;
            default: printf("无效的选择，请重新输入。\n"); break;
        }

        if (subChoice != 0) {
            printf("\n按回车键继续...\n");
            clearInputBuffer();
        }
    } while (subChoice != 0);
}

// 添加新学生
void addStudent() {
    printf("\n--- 添加学生 ---\n");
    if (studentCount >= MAX_STUDENTS) {
        printf("学生数量已达到上限。\n");
        return;
    }
    int i = studentCount;
    printf("请输入学号: ");
    while (scanf("%lld", &students[i].id) != 1) {
        printf("输入无效，请输入一个整数作为学号: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    // 检查学号是否已存在
    if (findStudentById(students[i].id) != -1) {
        printf("错误：学号 %lld 已存在，请使用其他学号。\n", students[i].id);
        return;
    }
    sprintf(students[i].password, "%lld", students[i].id); // 默认密码与学号相同
    printf("请输入姓名 (无空格): ");
    scanf("%19s", students[i].name);
    clearInputBuffer();
    printf("请依次输入各科成绩 (0-100):\n");
    for (int j = 0; j < MAX_SUBJECTS; j++) {
        printf("请输入 %s 成绩: ", SUBJECT_NAMES[j]);
        while (scanf("%f", &students[i].scores[j]) != 1 ||
               students[i].scores[j] < 0 || students[i].scores[j] > 100) {
            printf("输入无效，请输入一个0到100之间的数字: ");
            clearInputBuffer();
        }
        clearInputBuffer();
    }
    studentCount++;
    printf("学生添加成功！默认密码与学号相同。\n");
}

// 根据学号查找学生，返回索引；未找到返回-1
int findStudentById(long long id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            return i;
        }
    }
    return -1;
}

// 删除指定学号的学生
void deleteStudent() {
    printf("\n--- 删除学生 ---\n");
    if (studentCount == 0) {
        printf("当前没有学生信息可删除。\n");
        return;
    }
    long long id_to_delete;
    printf("请输入要删除的学生的学号: ");
    while (scanf("%lld", &id_to_delete) != 1) {
        printf("输入无效，请输入一个整数作为学号: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    int index = findStudentById(id_to_delete);
    if (index == -1) {
        printf("未找到学号为 %lld 的学生。\n", id_to_delete);
        return;
    }
    // 将后续元素前移，覆盖要删除的元素
    for (int i = index; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }
    studentCount--;
    printf("学号为 %lld 的学生已成功删除。\n", id_to_delete);
}

// 修改指定学号的学生信息
void modifyStudent() {
    printf("\n--- 修改学生信息 ---\n");
    if (studentCount == 0) {
        printf("当前没有学生信息可修改。\n");
        return;
    }
    long long id_to_modify;
    printf("请输入要修改的学生的学号: ");
    while (scanf("%lld", &id_to_modify) != 1) {
        printf("输入无效，请输入一个整数作为学号: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    int index = findStudentById(id_to_modify);
    if (index == -1) {
        printf("未找到学号为 %lld 的学生。\n", id_to_modify);
        return;
    }
    printf("找到了学生！当前信息如下：\n");
    printf("%-10s: %lld\n", "学号", students[index].id);
    printf("%-10s: %s\n", "姓名", students[index].name);
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("  - %-8s: %.2f\n", SUBJECT_NAMES[i], students[index].scores[i]);
    }
    printf("\n请开始输入新信息 (密码将自动更新为新学号)：\n");
    printf("请输入新的学号: ");
    long long new_id;
    while (scanf("%lld", &new_id) != 1) {
        printf("输入无效，请输入一个整数作为学号: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    // 检查新学号是否冲突
    if (new_id != students[index].id && findStudentById(new_id) != -1) {
        printf("错误：新学号 %lld 已被其他学生使用，修改失败。\n", new_id);
        return;
    }
    students[index].id = new_id;
    sprintf(students[index].password, "%lld", students[index].id); // 更新密码
    printf("请输入新的姓名 (无空格): ");
    scanf("%19s", students[index].name);
    clearInputBuffer();
    printf("请依次输入新的各科成绩 (0-100):\n");
    for (int j = 0; j < MAX_SUBJECTS; j++) {
        printf("请输入新的 %s 成绩: ", SUBJECT_NAMES[j]);
        while (scanf("%f", &students[index].scores[j]) != 1 ||
               students[index].scores[j] < 0 || students[index].scores[j] > 100) {
            printf("输入无效，请输入一个0到100之间的数字: ");
            clearInputBuffer();
        }
        clearInputBuffer();
    }
    printf("学生信息修改成功！\n");
}

// 查询指定学号的学生信息
void searchStudent() {
    printf("\n--- 查询学生信息 ---\n");
    if (studentCount == 0) {
        printf("当前没有学生信息可查询。\n");
        return;
    }
    long long id_to_search;
    printf("请输入要查询的学生的学号: ");
    while (scanf("%lld", &id_to_search) != 1) {
        printf("输入无效，请输入一个整数作为学号: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    int index = findStudentById(id_to_search);
    if (index == -1) {
        printf("未找到学号为 %lld 的学生。\n", id_to_search);
    } else {
        printf("找到了学生！信息如下：\n");
        printf("%-10s: %lld\n", "学号", students[index].id);
        printf("%-10s: %s\n", "姓名", students[index].name);
        printf("各科成绩:\n");
        for (int i = 0; i < MAX_SUBJECTS; i++) {
             printf("  - %-8s: %.2f\n", SUBJECT_NAMES[i], students[index].scores[i]);
        }
        float average_score = calculateAverage(&students[index]);
        printf("------------------------\n");
        printf("%-10s: %.2f\n", "平均分", average_score);
    }
}

// 按默认顺序显示所有学生
void listStudents() {
    displayStudentList(students, studentCount, "--- 学生信息列表 (按序号) ---");
}

// 排名功能子菜单
void rankMenu() {
    int choice;
    do {
        printf("\n----- 成绩排名子菜单 -----\n");
        printf("1. 按序号排名 (默认)\n");
        printf("2. 按平均分排名\n");
        printf("3. 按单科成绩排名\n");
        printf("0. 返回主菜单\n");
        printf("---------------------------\n");
        printf("请输入您的选择: ");

        while (scanf("%d", &choice) != 1) {
            printf("输入无效，请输入一个数字: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        switch (choice) {
            case 1: listStudents();    break;
            case 2: rankByAverage();   break;
            case 3: rankBySubject();   break;
            case 0: printf("返回主菜单。\n"); break;
            default: printf("无效的选择，请重新输入。\n"); break;
        }

        if (choice != 0) {
            printf("\n按回车键继续...\n");
            clearInputBuffer();
        }
    } while (choice != 0);
}

// 按平均分对学生进行排名并显示
void rankByAverage() {
    if (studentCount == 0) {
        printf("当前没有学生信息可排名。\n");
        return;
    }

    // 创建临时数组，避免修改原始数据顺序
    struct Student tempStudents[MAX_STUDENTS];
    for (int i = 0; i < studentCount; i++) {
        tempStudents[i] = students[i];
    }

    // 冒泡排序 (按平均分降序)
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            if (calculateAverage(&tempStudents[j]) < calculateAverage(&tempStudents[j+1])) {
                swapStudents(&tempStudents[j], &tempStudents[j+1]);
            }
        }
    }

    // 显示排序结果
    displayStudentList(tempStudents, studentCount, "--- 学生成绩列表 (按平均分排名) ---");
}

// 按指定单科成绩对学生进行排名并显示
void rankBySubject() {
    if (studentCount == 0) {
        printf("当前没有学生信息可排名。\n");
        return;
    }

    // 用户选择排名的科目
    int subject_choice;
    printf("\n请选择要排名的科目:\n");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("%d. %s\n", i + 1, SUBJECT_NAMES[i]);
    }
    printf("请输入科目序号: ");
    while (scanf("%d", &subject_choice) != 1 || subject_choice < 1 || subject_choice > MAX_SUBJECTS) {
        printf("输入无效，请输入 1 到 %d 之间的数字: ", MAX_SUBJECTS);
        clearInputBuffer();
    }
    clearInputBuffer();
    int subject_index = subject_choice - 1;

    // 创建临时数组
    struct Student tempStudents[MAX_STUDENTS];
    for (int i = 0; i < studentCount; i++) {
        tempStudents[i] = students[i];
    }

    // 冒泡排序 (按指定科目成绩降序)
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            if (tempStudents[j].scores[subject_index] < tempStudents[j+1].scores[subject_index]) {
                swapStudents(&tempStudents[j], &tempStudents[j+1]);
            }
        }
    }

    // 显示排序结果
    char title[100];
    sprintf(title, "--- 学生成绩列表 (按%s排名) ---", SUBJECT_NAMES[subject_index]);
    displayStudentList(tempStudents, studentCount, title);
}

// 通用函数：显示学生列表 (带表头和排名)
void displayStudentList(const struct Student list[], int count, const char* title) {
    printf("\n%s\n", title);
    if (count == 0) {
        printf("当前没有学生信息可显示。\n");
        return;
    }

    // 打印表头
    printf("%-4s%-16s%-12s", "排名", "学号", "姓名");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("%-10s", SUBJECT_NAMES[i]);
    }
    printf("%-10s\n", "平均分");
    printf("----------------------------------------------------------------------------\n");

    // 打印学生信息
    for (int i = 0; i < count; i++) {
        printf("%-4d%-16lld%-12s", i + 1, list[i].id, list[i].name);

        for (int j = 0; j < MAX_SUBJECTS; j++) {
            printf("%-10.2f", list[i].scores[j]);
        }

        float average_score = calculateAverage(&list[i]);
        printf("%-10.2f\n", average_score);
    }

    printf("----------------------------------------------------------------------------\n");
    printf("总计 %d 名学生。\n", count);
}


// --- 辅助函数 ---

// 计算单个学生的平均分
float calculateAverage(const struct Student* s) {
    if (MAX_SUBJECTS == 0) return 0.0f;
    float total = 0;
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        total += s->scores[i];
    }
    return total / MAX_SUBJECTS;
}

// 交换两个学生结构体的数据
void swapStudents(struct Student* a, struct Student* b) {
    struct Student temp = *a;
    *a = *b;
    *b = temp;
}


// --- 文件操作 ---

// 保存学生数据到文件
void saveData() {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("错误：无法打开文件 %s 进行写入。\n", FILENAME);
        return;
    }
    fprintf(fp, "%d\n", studentCount); // 首先写入学生总数
    for (int i = 0; i < studentCount; i++) {
        fprintf(fp, "%lld %s", students[i].id, students[i].name);
        for (int j = 0; j < MAX_SUBJECTS; j++) {
            fprintf(fp, " %.2f", students[i].scores[j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("数据已成功保存到文件 %s。\n", FILENAME);
}

// 从文件加载学生数据
void loadData() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) { // 文件不存在是正常情况，不报错
        studentCount = 0;
        return;
    }
    int fileStudentCount = 0;
    if (fscanf(fp, "%d\n", &fileStudentCount) != 1) { // 读取学生总数
        studentCount = 0;
        fclose(fp);
        return;
    }
    if (fileStudentCount > MAX_STUDENTS) { // 防止溢出
        fileStudentCount = MAX_STUDENTS;
    }
    studentCount = 0;
    for (int i = 0; i < fileStudentCount; i++) {
        // 读取一行学生数据
        if (fscanf(fp, "%lld %19s", &students[i].id, students[i].name) == 2) {
            int success = 1;
            for (int j = 0; j < MAX_SUBJECTS; j++) {
                if (fscanf(fp, "%f", &students[i].scores[j]) != 1) {
                    success = 0; // 如果成绩读取失败，标记此行数据无效
                    break;
                }
            }
            if (success) {
                sprintf(students[i].password, "%lld", students[i].id); // 自动生成密码
                studentCount++;
            } else {
                printf("警告：文件 %s 格式错误，加载部分数据。\n", FILENAME);
                break; // 停止加载以防更多错误
            }
        } else {
            break; // 无法读取学号和姓名，停止加载
        }
    }
    fclose(fp);
}

// 从文件加载教师数据
void loadTeachers() {
    FILE *fp = fopen(TEACHER_FILENAME, "r");
    if (fp == NULL) {
        printf("提示: 未找到教师列表文件 %s，将使用默认教师账号 (admin/password)。\n", TEACHER_FILENAME);
        teacherCount = 0;
        return;
    }
    teacherCount = 0;
    // 循环读取教师账号密码，直到文件末尾或达到最大数量
    while (teacherCount < MAX_TEACHERS && fscanf(fp, "%s %s", teachers[teacherCount].username, teachers[teacherCount].password) == 2) {
        teacherCount++;
    }
    fclose(fp);
}