#include <mutex>
#include <iostream>
using namespace std;


struct Node
{
	int value;
	Node* next;
	std::mutex* node_mutex;
};

class FineGrainedQueue
{   
    private:

	Node* head = nullptr;
	std::mutex queue_mutex;


    public:

    void insertIntoMiddle(int value, int pos) {
        if (pos < 1){
            cout << "Значение pos противоречит условию задания\n";
            return;
        }

        Node* prev, * current;
        queue_mutex.lock();

        if (!this->head) {
            // хотя по условию "очередь не пустая", на всякий случай
            queue_mutex.unlock();
            cout << "Очередь пуста - противоречит условию задания\n";
            return;
        }


        prev = this->head;
        current = this->head->next;

        prev->node_mutex->lock();
        queue_mutex.unlock();

        if (current) // проверили и только потом залочили
            current->node_mutex->lock();
        int current_pos = 1;

        
        // поиск
        while (current)
        {
            if (current_pos == pos)
            {
                Node* insert = new Node{ value, nullptr, new std::mutex() };

                insert->next = current;
                prev->next = insert;

                prev->node_mutex->unlock();
                current->node_mutex->unlock();

                return;
            }
            Node* old_prev = prev;
            prev = current;
            current = current->next;
            old_prev->node_mutex->unlock();
            if (current) // проверили и только потом залочили
                current->node_mutex->lock();
            
            current_pos++;
        }
        
        // вставка в конец
        Node* insert = new Node{ value, nullptr, new std::mutex() };
        prev->next = insert;
        prev->node_mutex->unlock();
    }


    void test_print() {
        if (!head) return;

        Node* node_print = this->head;

        while (node_print) {
            cout << "[ " << node_print->value << " ] ";
            node_print = node_print->next;
        }
        cout << "\n";
    }


    void test_push(int value) {
        if (this->head == nullptr) {
            this->head = new Node{ value, nullptr, new std::mutex() };
            return;
        }

        Node* node_current = this->head;
        while (node_current) {
            if (!node_current->next)
                break;
            node_current = node_current->next;
        }
        node_current->next = new Node{ value, nullptr, new std::mutex() };
    }
};




int main()
{
	setlocale(LC_ALL, "Russian");

    FineGrainedQueue FGQ;
    
    FGQ.insertIntoMiddle(99, 2);
    cout << "Заполнение очереди\n";
    FGQ.test_push(1);
    FGQ.test_push(2);
    FGQ.test_push(3);
    FGQ.test_push(4);

    FGQ.test_print();

    cout << "insertIntoMiddle: value - 99; pos - 2\n";
    FGQ.insertIntoMiddle(99, 2);

    FGQ.test_print();

    cout << "insertIntoMiddle: value - 55; pos - 7\n";
    FGQ.insertIntoMiddle(55, 7);

    FGQ.test_print();
    
    cout << "insertIntoMiddle: value - 0; pos - 0\n";
    FGQ.insertIntoMiddle(0, 0);

    FGQ.test_print();
	return 0;
}
