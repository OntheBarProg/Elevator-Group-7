#include <iostream>
#include <fstream>
using namespace std;

struct elev {
    int floornum;
    int direction;
};

struct node {
    elev data;
    node *next;
};

class queue {
private:
    node *front;
    node *rear;

public:
    queue() {
        front = NULL;
        rear = NULL;
    }

    void enqueueWithPriority(elev x, int currentFloor, int direction) {
        node *temp = new node;
        temp->data = x;
        temp->next = NULL;

        if (front == NULL) {
            front = rear = temp;
            return;
        }

        node *prev = NULL, *curr = front;
        while (curr != NULL) {
            int currFloorDiff = abs(curr->data.floornum - currentFloor);
            int newFloorDiff = abs(x.floornum - currentFloor);

            if ((x.direction == direction && x.floornum * direction >= currentFloor * direction) &&
                newFloorDiff < currFloorDiff) {
                break;
            }

            prev = curr;
            curr = curr->next;
        }

        if (prev == NULL) {
            temp->next = front;
            front = temp;
        } else {
            prev->next = temp;
            temp->next = curr;
            if (curr == NULL) {
                rear = temp;
            }
        }
    }

    void dequeue() {
        if (front == NULL) {
            return;
        }
        node *temp = front;
        front = front->next;
        if (front == NULL) {
            rear = NULL;
        }
        delete temp;
    }

    elev Front() {
        return front->data;
    }

    bool isEmpty() {
        return front == NULL;
    }
};

class Stack {
private:
    struct StackNode {
        int data;
        StackNode *next;
    };

    StackNode *topNode;

public:
    Stack() {
        topNode = NULL;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(int x) {
        StackNode *newNode = new StackNode;
        newNode->data = x;
        newNode->next = topNode;
        topNode = newNode;
    }

    void pop() {
        if (topNode == NULL) {
            return;
        }
        StackNode *temp = topNode;
        topNode = topNode->next;
        delete temp;
    }

    int top() {
        if (topNode == NULL) {
            throw runtime_error("Stack is empty");
        }
        return topNode->data;
    }

    bool isEmpty() {
        return topNode == NULL;
    }
};

class elevator {
private:
    int currentfloor;
    int direction;
    queue q;
    Stack visitedFloors;
    ofstream logFile;
    int buildingsize[9];

public:
    elevator() {
        currentfloor = 0;
        direction = 1;
        logFile.open("elevator_log.txt", ios::out);
        for (int i = 0; i < 9; i++) {
            buildingsize[i] = 0;
        }
    }

    ~elevator() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void move() {
        if (q.isEmpty()) {
            cout << "No requests in the queue. Elevator is idle.\n";
            logActivity("Elevator is idle. No requests.");
            return;
        }

        elev x = q.Front();
        q.dequeue();
        if (x.floornum > currentfloor) {
            direction = 1;
        } else if (x.floornum < currentfloor) {
            direction = -1;
        }
        currentfloor = x.floornum;
        visitedFloors.push(currentfloor);
        logActivity("Elevator moved to floor " + to_string(currentfloor));
        buildingsize[currentfloor] = 0;
        cout << "Elevator is at floor " << currentfloor << endl;

        if (q.isEmpty()) {
            for (int i = 0; i < 9; i++) {
                if (buildingsize[i] != 0) {
                    direction = (i > currentfloor) ? 1 : -1;
                    break;
                }
            }
        }
    }

    void requestPickup(int floordes, int reqDirection) {
        if (floordes < 0 || floordes >= 9) {
            cout << "Invalid floor request.\n";
            logActivity("Invalid pickup request for floor " + to_string(floordes));
            return;
        }
        if (buildingsize[floordes] == 1) {
            cout << "Floor " << floordes << " is already in the queue.\n";
            return;
        }
        elev x;
        x.floornum = floordes;
        x.direction = reqDirection;
        q.enqueueWithPriority(x, currentfloor, direction);
        buildingsize[floordes] = 1;
        logActivity("Pickup request added for floor " + to_string(floordes) + " going " + (reqDirection == 1 ? "up" : "down"));
        cout << "Pickup request added for floor " << floordes << " going " << (reqDirection == 1 ? "up" : "down") << endl;
    }

    void requestDestination(int floordes) {
        if (floordes < 0 || floordes >= 9) {
            cout << "Invalid destination floor.\n";
            logActivity("Invalid destination request for floor " + to_string(floordes));
            return;
        }
        if (buildingsize[floordes] == 1) {
            cout << "Destination floor " << floordes << " is already in the queue.\n";
            return;
        }
        elev x;
        x.floornum = floordes;
        x.direction = direction;
        q.enqueueWithPriority(x, currentfloor, direction);
        buildingsize[floordes] = 1;
        logActivity("Destination request added for floor " + to_string(floordes));
        cout << "Destination request added for floor " << floordes << endl;
    }

    void displayState() {
        cout << "\n--- Elevator State ---\n";
        cout << "Current floor: " << currentfloor << ", Direction: " << (direction == 1 ? "Up" : "Down") << "\n";
        cout << "Building floors state:\n";
        for (int i = 0; i < 9; i++) {
            cout << "Floor " << i << ": " << (buildingsize[i] ? "Requested" : "Idle") << endl;
        }
        displayVisitedFloors();
    }

    void displayCurrentState() const {
        cout << "\n[Elevator is currently at floor " << currentfloor << ", going "
             << (direction == 1 ? "Up" : "Down") << "]\n";
    }

    void displayVisitedFloors() {
        cout << "Visited floors: ";
        Stack temp = visitedFloors;
        while (!temp.isEmpty()) {
            cout << temp.top() << " ";
            temp.pop();
        }
        cout << endl;
    }

    void logActivity(const string &message) {
        if (logFile.is_open()) {
            logFile << message << endl;
        }
    }
};

bool getValidInput(int &input) {
    string userInput;
    cin >> userInput;
    bool isValid = true;

    for (char c : userInput) {
        if (!isdigit(c) && c != '-') {
            isValid = false;
            break;
        }
    }

    if (!isValid) {
        cout << "Invalid input. Please enter a valid number.\n";
        return false;
    }

    input = stoi(userInput);
    return true;
}

int main() {
    elevator a;
    int choice, floordes, reqDirection;

    while (true) {
        a.displayCurrentState();

        cout << "\n--- Elevator System ---\n";
        cout << "1. Request elevator pickup\n";
        cout << "2. Enter destination floor\n";
        cout << "3. Move elevator\n";
        cout << "4. Display elevator state\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        if (!getValidInput(choice)) {
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter the floor you want to request: ";
            if (!getValidInput(floordes)) {
                continue;
            }
            cout << "Enter direction (1 for up, -1 for down): ";
            if (!getValidInput(reqDirection)) {
                continue;
            }
            a.requestPickup(floordes, reqDirection);
            break;
        case 2:
            cout << "Enter your destination floor: ";
            if (!getValidInput(floordes)) {
                continue;
            }
            a.requestDestination(floordes);
            break;
        case 3:
            a.move();
            break;
        case 4:
            a.displayState();
            break;
        case 5:
            cout << "Exiting the system. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
