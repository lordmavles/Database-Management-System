#include <bits/stdc++.h>
using namespace std;

const int M = 2;

class Bucket
{
    int depth, size, cont[M];
    public:
    int insert_item(int x);
    int remove_item(int x);
    int search_item(int x);
    int is_full();
    int is_empty();
    int set_depth(int x);
    int set_size(int x);
    int get_depth();
    int get_size();
    int* get_items();
};

int global_depth, mod;
Bucket** dir = NULL;

int hash_fun(int x);
int split_bucket(int x);
int merge_bucket(int x);
int double_dir();
int halve_dir();
int insert_item(int x);
int remove_item(int x);
int search_item(int x);
int update_item(int x, int y);
int get_bucket();
int get_bucketcont();
int interface();
void init();

int main()
{
    cout << "Start" << endl;
    init();
    interface();
    cout << "End" << endl;
    return 0;
}

int interface()
{
    system("cls");
    cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
    while(true) {
        cout << "Enter : " << endl;
        cout << "   1 for insertion" << endl;
        cout << "   2 for deletion" << endl;
        cout << "   3 for search" << endl;
        cout << "   4 for updation" << endl;
        cout << "   5 for no. of buckets" << endl;
        cout << "   6 get contents of every directory" << endl;
        cout << "   7 for global depth" << endl;
        cout << "   8 to exit" << endl;
        int x; cin >> x;
        if(x == 8) break;
        if(x == 1) {
            cout << "Enter value to be inserted : ";
            cin >> x;
            cout << endl;
            if(insert_item(x)) cout << "Successfully inserted";
            else cout << "Failed to insert";
        }
        else if(x == 2) {
            cout << "Enter value to be deleted : ";
            cin >> x;
            if(remove_item(x)) cout << "Successfully removed";
            else cout << "Failed to remove";
        }
        else if(x == 3) {
            cout << "Enter value to be searched : ";
            cin >> x;
            int temp = search_item(x);
            if(temp != -1) cout << "Value found in index : " << temp;
            else cout << "Value not found";
        }
        else if(x == 4) {
            cout << "Enter the value to be updated followed by the new value : ";
            int y; cin >> x >> y;
            if(update_item(x, y)) cout << "Successfully updated";
            else cout << "Failed to update";
        }
        else if(x == 5) cout << get_bucket() << endl;
        else if(x == 6) cout << get_bucketcont() << endl;
        else if(x == 7) cout << global_depth << endl;
        else cout << "Invalid Input";
        cout << endl << endl;
    }
    return 1;
}

void init()
{
    global_depth = 0;
    mod = pow(2, global_depth);
    dir = (Bucket **)malloc(sizeof(Bucket *));
    dir[0] = (Bucket *)malloc(sizeof(Bucket));
    dir[0]->set_size(0), dir[0]->set_depth(0);
    return;
}

int get_bucketcont()
{
    for(int i = 0; i < mod; i++) {
        int *hold = dir[i]->get_items(), size = dir[i]->get_size();
        for(int j = 0; j < size; j++) cout << hold[j] << ' '; cout << endl;
        cout << size << endl;
    }
    return 1;
}

int get_bucket()
{
    set <Bucket *> X;
    for(int i = 0; i < mod; i++) X.insert(dir[i]);
    return X.size();
}

int search_item(int x)
{
    int temp = hash_fun(x);
    return (dir[temp%mod]->search_item(x) == -1 ? -1 : temp%mod);
}

int update_item(int x, int y)
{
    if(search_item(x) == -1) return 0;
    remove_item(x);
    insert_item(y);
    return 1;
}

int remove_item(int x)
{
    if(search_item(x) == -1) return 0;
    int temp = hash_fun(x);
    dir[temp%mod]->remove_item(x);
    while(merge_bucket(x)) halve_dir();
    return 1;
}

int insert_item(int x)
{
    if(search_item(x) != -1) return 0;
    int temp = hash_fun(x);
    while(dir[temp%mod]->is_full()) {
        if(dir[temp%mod]->get_depth() == global_depth) double_dir();
        split_bucket(x);
    }
    dir[temp%mod]->insert_item(x);
    return 1;
}

int double_dir()
{
    Bucket** temp = dir;
    dir = NULL;
    dir = (Bucket **)malloc(2*mod*sizeof(Bucket *));
    for(int i = 0; i < 2*mod; i++) dir[i] = temp[i%mod];
    free(temp);
    global_depth++;
    mod *= 2;
    return 1;
}

int halve_dir()
{
    int cc = 0;
    for(int i = 0; i < mod; i++) cc = max(cc, dir[i]->get_depth());
    if(cc >= global_depth) return 1;
    Bucket** temp = dir;
    dir = NULL;
    mod /= 2;
    dir = (Bucket **)malloc(mod*sizeof(Bucket *));
    for(int i = 0; i < mod; i++) dir[i] = temp[i];
    free(temp);
    global_depth--;
    return 1;
}

int hash_fun(int x)
{
    return x;
}

int merge_bucket(int x)
{
    int depth = dir[hash_fun(x)%mod]->get_depth();
    if(depth == 0) return 0;
    int cur_mod = pow(2, depth-1);
    int lb = hash_fun(x)%cur_mod, ub = cur_mod+hash_fun(x)%cur_mod;
    int depth1 = dir[lb]->get_depth(), depth2 = dir[ub]->get_depth();
    int size1 = dir[lb]->get_size(), size2 = dir[ub]->get_size();
    if(depth1 != depth2) return 0;
    if(size1+size2 > M) return 0;
    Bucket *b = (Bucket *)malloc(sizeof(Bucket)), *b1 = dir[lb], *b2 = dir[ub];
    b->set_depth(depth-1), b->set_size(0);
    int *hold1 = b1->get_items(),*hold2 = b2->get_items();
    for(int i = 0; i < mod; i++) {
        if((hash_fun(x)%cur_mod) == (i%cur_mod)) dir[i] = b;
    }
    free(b1), free(b2);
    for(int i = 0; i < size1; i++) b->insert_item(hold1[i]);
    for(int i = 0; i < size2; i++) b->insert_item(hold2[i]);
    free(hold1), free(hold2);
    return 1;
}

int split_bucket(int x)
{
    int depth = dir[hash_fun(x)%mod]->get_depth();
    if(depth == global_depth) return 0;
    int cur_mod = pow(2, depth);
    int size = dir[hash_fun(x)%mod]->get_size();
    Bucket *b = dir[hash_fun(x)%mod], *b1 = (Bucket *)malloc(sizeof(Bucket)), *b2 = (Bucket *)malloc(sizeof(Bucket));
    b1->set_depth(depth+1), b1->set_size(0);
    b2->set_depth(depth+1), b2->set_size(0);
    int *hold = b->get_items();
    int c = 0;
    for(int i = 0; i < mod; i++) {
        if((hash_fun(x)%cur_mod) == (i%cur_mod)) {
            if(c) dir[i] = b1, c = 0;
            else dir[i] = b2, c = 1;
        }
    }
    free(b);
    for(int i = 0; i < size; i++) dir[hash_fun(hold[i])%mod]->insert_item(hold[i]);
    free(hold);
    return 1;
}

int Bucket::insert_item(int x)
{
    if(this->search_item(x) != -1) return 0;
    if(this->is_full()) return 0;
    cont[this->get_size()] = x;
    this->set_size(this->get_size()+1);
    return 1;
}

int Bucket::remove_item(int x)
{
    int temp = this->search_item(x);
    if(temp == -1) return 0;
    swap(cont[temp], cont[this->get_size()-1]);
    this->set_size(this->get_size()-1);
    return 1;
}

int Bucket::search_item(int x)
{
    for(int i = 0; i < size; i++) {
        if(cont[i] == x) return i;
    }
    return -1;
}

int Bucket::is_full()
{
    return (size == M ? 1 : 0);
}

int Bucket::is_empty()
{
    return (size == 0 ? 1 : 0);
}

int Bucket::set_size(int x)
{
    if(x < 0 || x > M) return 0;
    size = x;
    return 1;
}

int Bucket::set_depth(int x)
{
    if(x < 0) return 0;
    depth = x;
    return 1;
}

int Bucket::get_depth()
{
    return depth;
}

int Bucket::get_size()
{
    return size;
}

int* Bucket::get_items()
{
    if(this->size == 0) return NULL;
    int* hold = (int *)malloc(this->get_size()*sizeof(int));
    for(int i = 0; i < this->get_size(); i++) hold[i] = cont[i];
    return hold;
}
