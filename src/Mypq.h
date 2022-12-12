#include <mutex>
#include <thread>
#include <iostream>
#include<stdlib.h>
#include<time.h>

enum Status {EMPTY,AVAILABLE,BUSY};

using namespace std;

template<class T>
struct Node
{
    Status tag;
    int score;
    T item;
    mutex candado;
    int owner;

    void init(T item1, int score1)
    {
        item = item1;
        score = score1;
        tag = BUSY;
        
    }
    bool amOwner(thread::id id)
    {
        if (id == this_thread::get_id())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    Node()
    {
        tag = EMPTY;
    }
};

template<class T>
struct Heap
{
    int ROOT = 0;
    int NO_ONE = -1;
    mutex heap_lock; 
    int size=0;
    int owner;
    Node<T> *heap;
    int capacity;

    Heap<T>(int capacity1)
    {
        size = ROOT;
        capacity = capacity1;
        heap = new Node<T>[capacity1 ];
        for (int x = 0; x < capacity1; x++)
        {
            heap[x].tag=EMPTY;
        }
    }

    bool Push(T item, int score)
    {
        heap_lock.lock();
        
        int child = size;
        int aux;
        size++;
        heap[child].candado.lock();
        heap[child].init(item, score);


        heap_lock.unlock();
        heap[child].candado.unlock();

        while (ROOT < child)
        {
            
            int parent = (child-1) / 2;
            heap[parent].candado.lock();
            heap[child].candado.lock();
            int oldChild = child;
            
            if (heap[parent].tag == AVAILABLE && heap[child].amOwner(this_thread::get_id()))
            {
                if (heap[child].score < heap[parent].score) 
                {
                    //swap
                    aux = heap[child].score;
                    heap[child].score = heap[parent].score;
                    heap[parent].score=aux;

                    aux = heap[child].item;
                    heap[child].item = heap[parent].item;
                    heap[parent].item = aux;
                    //swap

                    heap[child].tag = AVAILABLE;
                    child = parent;
                }
                else
                {
                    heap[child].tag = AVAILABLE;
                    heap[child].owner = NO_ONE;

                    heap[child].candado.unlock();
                    heap[parent].candado.unlock();
                    return true;
                }

            }
            else if (!heap[child].amOwner(this_thread::get_id()))
            {
                child = parent;
            }
            heap[oldChild].candado.unlock();
            heap[parent].candado.unlock();
        }



        if (ROOT == child)
        {
            heap[ROOT].candado.lock();
            if (heap[ROOT].amOwner(this_thread::get_id()))
            {
                heap[ROOT].tag = AVAILABLE;
                heap[child].owner = NO_ONE;
            }
            heap[ROOT].candado.unlock();
            return true;
        }
    }

    T PopTop()
    {
        heap_lock.lock();
        int bottom = size;
        if (size == 0)
        {
            heap_lock.unlock();
            return -1;
        }
        else
        {
            size--;
        }
        
        //cout << "size " << size <<endl;
        
        int aux;
        Status aux1;
        
        heap[ROOT].candado.lock();
        heap[bottom].candado.lock();
        heap_lock.unlock();
        T item = heap[ROOT].item;
        heap[ROOT].tag = EMPTY;
        heap[ROOT].owner = NO_ONE;
        
        //cout << "root " << ROOT << endl;
        //cout << "bottom " << bottom << endl;

        //swap

        aux = heap[ROOT].score;
        heap[ROOT].score = heap[bottom].score;
        heap[bottom].score = aux;

        aux = heap[ROOT].item;
        heap[ROOT].item = heap[bottom].item;
        heap[bottom].item = aux;

        aux1 = heap[ROOT].tag;
        heap[ROOT].tag = heap[bottom].tag;
        heap[bottom].tag = aux1;
        
        //swap

        heap[bottom].candado.unlock();

        //cout << "root " << ROOT << endl;
        //cout << "bottom " << bottom << endl;

        if (heap[ROOT].tag == EMPTY)
        {
            //cout << "jola" << endl;
            heap[ROOT].candado.unlock();
            
            return item;
        }

        //cout << "root " << ROOT << endl;
        //cout << "bottom " << bottom << endl;
        int child = 0;
        int parent = ROOT;


        heap[parent].candado.unlock();

        //cout << "root " << ROOT << endl;
        //cout << "bottom " << bottom << endl;

        while (parent < size / 2)
        {
            int oldparent = parent;
            int left = parent * 2 + 1;
            int right = parent * 2 + 2;
            heap[parent].candado.lock();
            heap[left].candado.lock();
            heap[right].candado.lock();
            //cout << "parent " << parent << endl;
            //cout << "left " << left << endl;
            //cout << "right " << right << endl;

            if (heap[left].tag == EMPTY)
            {
                heap[right].candado.unlock();
                heap[left].candado.unlock();
                heap[parent].candado.unlock();
                break;
            }
            else if (heap[right].tag == EMPTY || heap[left].score < heap[right].score)
            {
                //cout << "hey" << endl;
                heap[right].candado.unlock();
                child = left;

            }
            else
            {
                heap[left].candado.unlock();
                child = right;
            }
            if (heap[child].score < heap[parent].score && heap[child].tag != EMPTY)
            {
                //swap
                aux = heap[child].score;
                heap[child].score = heap[parent].score;
                heap[parent].score = aux;

                aux = heap[child].item;
                heap[child].item = heap[parent].item;
                heap[parent].item = aux;
                //swap

                heap[parent].tag = AVAILABLE;
                parent = child;
                
            }
            else
            {
                heap[child].candado.unlock();
                heap[parent].candado.unlock();
                break;
            }
            heap[oldparent].candado.unlock();
            heap[child].candado.unlock();
        }
        
        return item;


    }
    void print()
    {
        for (int x = 0; x < size; x++)
        {
            cout << heap[x].score<<" ";
        }
    }
};





