#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <conio.h>
#include <windows.h>

using namespace std;

enum TaskStatus {
    PENDING,
    IN_PROGRESS,
    COMPLETED
};

struct Task {
    string title;
    string description;
    TaskStatus status;
    string assignedTo;
};

struct Project {
    string name;
    string description;
    vector<Task> tasks;
    vector<string> teamMembers;
};

struct TeamMember {
    string username;
    string email;
    string password;
};

void setColor(int textColor, int bgColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | textColor);
}

class ProjectManagementSystem {
public:
    ProjectManagementSystem() : currentUser(nullptr) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    }

    void run() {
        while (true) {
            if (currentUser == nullptr) {
                loginMenu();
            } else {
                mainMenu();
            }
        }
    }

private:
    vector<Project> projects;
    vector<TeamMember> teamMembers;
    TeamMember* currentUser;

    void loginMenu() {
        system("cls");
        setColor(10, 0);
        cout << "Welcome to the Project Management System\n";
        setColor(15, 0);
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        char choice = _getch();
        switch (choice) {
            case '1': registerUser(); break;
            case '2': loginUser(); break;
            case '3': exit(0);
        }
    }

    void mainMenu() {
        system("cls");
        setColor(14, 0);
        cout << "Project Management System - Main Menu\n";
        setColor(15, 0);
        cout << "1. Add Project\n";
        cout << "2. View Projects\n";
        cout << "3. Add Task\n";
        cout << "4. View Tasks\n";
        cout << "5. Edit Project\n";
        cout << "6. Delete Project\n";
        cout << "7. Edit Task\n";
        cout << "8. Delete Task\n";
        cout << "9. List Team Members\n";
        cout << "10. Logout\n";
        cout << "11. Exit\n";
        char choice = _getch();
        switch (choice) {
            case '1': addProject(); break;
            case '2': viewProjects(); break;
            case '3': addTask(); break;
            case '4': viewTasks(); break;
            case '5': editProject(); break;
            case '6': deleteProject(); break;
            case '7': editTask(); break;
            case '8': deleteTask(); break;
            case '9': listTeamMembers(); break;
            case '10': currentUser = nullptr; break;
            case '11': exit(0);
        }
    }

    void registerUser() {
        system("cls");
        TeamMember member;
        setColor(12, 0);
        cout << "Register\n";
        setColor(15, 0);
        cout << "Username: ";
        cin >> member.username;
        cout << "Email: ";
        cin >> member.email;
        cout << "Password: ";
        cin >> member.password;
        teamMembers.push_back(member);
        setColor(10, 0);
        cout << "User registered successfully. Press any key to return to login menu.";
        _getch();
    }

    void loginUser() {
        system("cls");
        string username, password;
        setColor(12, 0);
        cout << "Login\n";
        setColor(15, 0);
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        auto it = find_if(teamMembers.begin(), teamMembers.end(), [&username, &password](const TeamMember& m) {
            return m.username == username && m.password == password;
        });
        if (it != teamMembers.end()) {
            currentUser = &(*it);
            setColor(10, 0);
            cout << "Logged in successfully. Press any key to enter the main menu.";
        } else {
            setColor(12, 0);
            cout << "Login failed. Press any key to try again.";
        }
        _getch();
    }

    void addProject() {
        system("cls");
        Project project;
        setColor(14, 0);
        cout << "Add Project\n";
        setColor(15, 0);
        cout << "Name: ";
        cin.ignore();
        getline(cin, project.name);
        cout << "Description: ";
        getline(cin, project.description);

        int numMembers;
        cout << "Number of team members: ";
        cin >> numMembers;
        cin.ignore();
        for (int i = 0; i < numMembers; ++i) {
            string member;
            cout << "Enter username of team member " << i + 1 << ": ";
            getline(cin, member);
            project.teamMembers.push_back(member);
        }

        projects.push_back(project);
        setColor(10, 0);
        cout << "Project added successfully. Press any key to return to the main menu.";
        _getch();
    }

    void viewProjects() {
        system("cls");
        setColor(14, 0);
        cout << "View Projects\n";
        setColor(15, 0);
        for (const auto& project : projects) {
            cout << "Name: " << project.name << endl;
            cout << "Description: " << project.description << endl;
            cout << "Team Members:\n";
            for (const auto& member : project.teamMembers) {
                cout << "  " << member << endl;
            }
            cout << "Tasks:\n";
            for (const auto& task : project.tasks) {
                cout << "  Title: " << task.title << ", Status: " << statusToString(task.status) << ", Assigned to: " << task.assignedTo << endl;
            }
            cout << endl;
        }
        cout << "Press any key to return to the main menu.";
        _getch();
    }

    void addTask() {
        system("cls");
        string projectName;
        cout << "Add Task\n";
        cout << "Enter project name: ";
        cin.ignore();
        getline(cin, projectName);
        auto projectIt = find_if(projects.begin(), projects.end(), [&projectName](const Project& p) {
            return p.name == projectName;
        });
        if (projectIt != projects.end()) {
            Task task;
            Project& project = *projectIt;
            cout << "Title: ";
            getline(cin, task.title);
            cout << "Description: ";
            getline(cin, task.description);
            task.status = PENDING;
            task.assignedTo = "";
            project.tasks.push_back(task);
            setColor(10, 0);
            cout << "Task added successfully. Press any key to return to the main menu.";
        } else {
            setColor(12, 0);
            cout << "Project not found. Press any key to return.";
        }
        _getch();
    }

    void viewTasks() {
        system("cls");
        string projectName;
        cout << "View Tasks\n";
        cout << "Enter project name: ";
        cin.ignore();
        getline(cin, projectName);
        auto projectIt = find_if(projects.begin(), projects.end(), [&projectName](const Project& p) {
            return p.name == projectName;
        });
        if (projectIt != projects.end()) {
            const Project& project = *projectIt;
            for (const auto& task : project.tasks) {
                cout << "Title: " << task.title << endl;
                cout << "Description: " << task.description << endl;
                cout << "Status: " << statusToString(task.status) << endl;
                cout << "Assigned to: " << task.assignedTo << endl;
                cout << endl;
            }
        } else {
            setColor(12, 0);
            cout << "Project not found. Press any key to return.";
        }
        _getch();
    }

    void editProject() {
        system("cls");
        string projectName;
        cout << "Edit Project\n";
        cout << "Enter project name: ";
        cin.ignore();
        getline(cin, projectName);
        auto projectIt = find_if(projects.begin(), projects.end(), [&projectName](const Project& p) {
            return p.name == projectName;
        });
        if (projectIt != projects.end()) {
            Project& project = *projectIt;
            cout << "New name (leave empty to keep current): ";
            string newName;
            getline(cin, newName);
            if (!newName.empty()) {
                project.name = newName;
            }
            cout << "New description (leave empty to keep current): ";
            string newDescription;
            getline(cin, newDescription);
            if (!newDescription.empty()) {
                project.description = newDescription;
            }
            setColor(10, 0);
            cout << "Project updated successfully. Press any key to return to the main menu.";
        } else {
            setColor(12, 0);
            cout << "Project not found. Press any key to return.";
        }
        _getch();
    }

    void deleteProject() {
        system("cls");
        string projectName;
        cout << "Delete Project\n";
        cout << "Enter project name: ";
        cin.ignore();
        getline(cin, projectName);
        auto projectIt = remove_if(projects.begin(), projects.end(), [&projectName](const Project& p) {
            return p.name == projectName;
        });
        if (projectIt != projects.end()) {
            projects.erase(projectIt, projects.end());
            setColor(10, 0);
            cout << "Project deleted successfully. Press any key to return to the main menu.";
        } else {
            setColor(12, 0);
            cout << "Project not found. Press any key to return.";
        }
        _getch();
    }

    void editTask() {
        system("cls");
        string projectName, taskTitle;
        cout << "Edit Task\n";
        cout << "Enter project name: ";
        cin.ignore();
        getline(cin, projectName);
        auto projectIt = find_if(projects.begin(), projects.end(), [&projectName](const Project& p) {
            return p.name == projectName;
        });
        if (projectIt != projects.end()) {
            Project& project = *projectIt;
            cout << "Enter task title: ";
            getline(cin, taskTitle);
            auto taskIt = find_if(project.tasks.begin(), project.tasks.end(), [&taskTitle](const Task& t) {
                return t.title == taskTitle;
            });
            if (taskIt != project.tasks.end()) {
                Task& task = *taskIt;
                cout << "New title (leave empty to keep current): ";
                string newTitle;
                getline(cin, newTitle);
                if (!newTitle.empty()) {
                    task.title = newTitle;
                }
                cout << "New description (leave empty to keep current): ";
                string newDescription;
                getline(cin, newDescription);
                if (!newDescription.empty()) {
                    task.description = newDescription;
                }
                cout << "New status (0: Pending, 1: In Progress, 2: Completed): ";
                int newStatus;
                cin >> newStatus;
                if (newStatus >= 0 && newStatus <= 2) {
                    task.status = static_cast<TaskStatus>(newStatus);
                }
                setColor(10, 0);
                cout << "Task updated successfully. Press any key to return to the main menu.";
            } else {
                setColor(12, 0);
                cout << "Task not found. Press any key to return.";
            }
        } else {
            setColor(12, 0);
            cout << "Project not found. Press any key to return.";
        }
        _getch();
    }

    void deleteTask() {
        system("cls");
        string projectName, taskTitle;
        cout << "Delete Task\n";
        cout << "Enter project name: ";
        cin.ignore();
        getline(cin, projectName);
        auto projectIt = find_if(projects.begin(), projects.end(), [&projectName](const Project& p) {
            return p.name == projectName;
        });
        if (projectIt != projects.end()) {
            Project& project = *projectIt;
            cout << "Enter task title: ";
            getline(cin, taskTitle);
            auto taskIt = remove_if(project.tasks.begin(), project.tasks.end(), [&taskTitle](const Task& t) {
                return t.title == taskTitle;
            });
            if (taskIt != project.tasks.end()) {
                project.tasks.erase(taskIt, project.tasks.end());
                setColor(10, 0);
                cout << "Task deleted successfully. Press any key to return to the main menu.";
            } else {
                setColor(12, 0);
                cout << "Task not found. Press any key to return.";
            }
        } else {
            setColor(12, 0);
            cout << "Project not found. Press any key to return.";
        }
        _getch();
    }

    void listTeamMembers() {
        system("cls");
        setColor(14, 0);
        cout << "List Team Members\n";
        setColor(15, 0);
        for (const auto& member : teamMembers) {
            cout << "Username: " << member.username << endl;
            cout << "Email: " << member.email << endl;
            cout << endl;
        }
        cout << "Press any key to return to the main menu.";
        _getch();
    }

    string statusToString(TaskStatus status) {
        switch (status) {
            case PENDING: return "Pending";
            case IN_PROGRESS: return "In Progress";
            case COMPLETED: return "Completed";
            default: return "Unknown";
        }
    }
};

int main() {
    ProjectManagementSystem pms;
    pms.run();
    return 0;
}
