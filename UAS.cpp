
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

struct Barang
{
    int id_barang;
    string nama;
    float harga;
    Barang *left;
    Barang *right;
    int tinggi;
};

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int tinggi(Barang *node)
{
    if (node == nullptr)
        return 0;
    return node->tinggi;
}

int seimbang(Barang *node)
{
    if (node == nullptr)
        return 0;
    return tinggi(node->left) - tinggi(node->right);
}

Barang *newNode(int id_barang, string nama, float harga)
{
    Barang *node = new Barang();
    node->id_barang = id_barang;
    node->nama = nama;
    node->harga = harga;
    node->left = nullptr;
    node->right = nullptr;
    node->tinggi = 1;
    return node;
}


Barang *rightrotate(Barang *y)
{
    Barang *x = y->left;
    Barang *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->tinggi = max(tinggi(y->left), tinggi(y->right)) + 1;
    x->tinggi = max(tinggi(x->left), tinggi(x->right)) + 1;

    return x;
}

Barang *leftrotate(Barang *x)
{
    Barang *y = x->right;
    Barang *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->tinggi = max(tinggi(x->left), tinggi(x->right)) + 1;
    y->tinggi = max(tinggi(y->left), tinggi(y->right)) + 1;

    return y;
}

Barang *minValueNode(Barang *node)
{
    Barang *current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

Barang *insertbst(Barang *root, int id_barang, string nama, float harga)
{
    Barang *node = newNode(id_barang, nama, harga);

    if (root == nullptr)
    {
        root = node;
        return root;
    }

    Barang *current = root;
    Barang *parent = nullptr;

    while (current != nullptr)
    {
        parent = current;

        if (id_barang < current->id_barang)
            current = current->left;
        else if (id_barang > current->id_barang)
            current = current->right;
        else
        {
            cout << "Barang dengan ID tersebut sudah ada." << endl;
            delete node;
            node = nullptr;
            return root;
        }
    }

    if (id_barang < parent->id_barang)
        parent->left = node;
    else
        parent->right = node;

    return root;
}

Barang *insertavl(Barang *root, int &id_barang, string nama, float &harga)
{
    Barang *node = newNode(id_barang, nama, harga);

    if (root == nullptr)
    {
        root = node;
        return root;
    }

    Barang *current = root;
    Barang *parent = nullptr;

    while (current != nullptr)
    {
        parent = current;

        if (nama < current->nama)
            current = current->left;
        else if (nama > current->nama)
            current = current->right;
        else
        {
            cout << "Barang dengan Id tersebut sudah ada." << endl;
            delete node; 
            return root;
        }
    }

    if (nama < parent->nama)
        parent->left = node;
    else
        parent->right = node;

    root->tinggi = 1 + max(tinggi(root->left), tinggi(root->right));
    int balance = seimbang(root);

    if (balance > 1 && nama < root->left->nama)
        return rightrotate(root);

    if (balance < -1 && nama > root->right->nama)
        return leftrotate(root);

    if (balance > 1 && nama > root->left->nama)
    {
        root->left = leftrotate(root->left);
        return rightrotate(root);
    }

    if (balance < -1 && nama < root->right->nama)
    {
        root->right = rightrotate(root->right);
        return leftrotate(root);
    }

    return root;
}


Barang *hapusbst(Barang *root, int id_barang, ofstream &file)
{
    if (root == nullptr)
        return root;

    if (id_barang < root->id_barang)
        root->left = hapusbst(root->left, id_barang, file);
    else if (id_barang > root->id_barang)
        root->right = hapusbst(root->right, id_barang, file);
    else
    {
        if (root->left == nullptr)
        {
            Barang *temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr)
        {
            Barang *temp = root->left;
            delete root;
            return temp;
        }

        Barang *temp = minValueNode(root->right);
        root->id_barang = temp->id_barang;
        root->nama = temp->nama;
        root->harga = temp->harga;
        root->right = hapusbst(root->right, temp->id_barang, file);
    }

    root->tinggi = 1 + max(tinggi(root->left), tinggi(root->right));
    int balance = seimbang(root);

    if (balance > 1 && seimbang(root->left) >= 0)
        return rightrotate(root);

    if (balance > 1 && seimbang(root->left) < 0)
    {
        root->left = leftrotate(root->left);
        return rightrotate(root);
    }

    if (balance < -1 && seimbang(root->right) <= 0)
        return leftrotate(root);

    if (balance < -1 && seimbang(root->right) > 0)
    {
        root->right = rightrotate(root->right);
        return leftrotate(root);
    }

    return root;
}
Barang *hapusavl(Barang *root, int &id_barang)
{
    if (root == nullptr)
        return root;

    if (id_barang < root->id_barang)
        root->left = hapusavl(root->left, id_barang);
    else if (id_barang > root->id_barang)
        root->right = hapusavl(root->right, id_barang);
    else
    {
        if ((root->left == nullptr) || (root->right == nullptr))
        {
            Barang *temp = root->left ? root->left : root->right;

            if (temp == nullptr)
            {
                delete root;
                return nullptr;
            }
            else
            {
                *root = *temp;
                delete temp;
            }
        }
        else
        {
            Barang *temp = minValueNode(root->right);
            root->id_barang = temp->id_barang;
            root->nama = temp->nama;
            root->harga = temp->harga;
            root->right = hapusavl(root->right, temp->id_barang);
        }
    }

    root->tinggi = 1 + max(tinggi(root->left), tinggi(root->right));
    int balance = seimbang(root);

    if (balance > 1 && seimbang(root->left) >= 0)
        return rightrotate(root);

    if (balance > 1 && seimbang(root->left) < 0)
    {
        root->left = leftrotate(root->left);
        return rightrotate(root);
    }

    if (balance < -1 && seimbang(root->right) <= 0)
        return leftrotate(root);

    if (balance < -1 && seimbang(root->right) > 0)
    {
        root->right = rightrotate(root->right);
        return leftrotate(root);
    }

    return root;
}


Barang *searchbst(Barang *root, int id_barang)
{
    if (root == nullptr || root->id_barang == id_barang)
        return root;

    if (id_barang < root->id_barang)
        return searchbst(root->left, id_barang);

    return searchbst(root->right, id_barang);
}
Barang *searchavl(Barang *root, int id_barang)
{
    if (root == nullptr || root->id_barang == id_barang)
        return root;

    if (id_barang < root->id_barang)
        return searchavl(root->left, id_barang);

    return searchavl(root->right, id_barang);
}

void simpanFile(Barang *root, ofstream &file)
{
    if (root != nullptr)
    {
        simpanFile(root->left, file);
        file << root->id_barang << " \"" << root->nama << "\" " << root->harga << endl;
        simpanFile(root->right, file);
    }
}
void inOrderTraversal(Barang *root)
{
    if (root != nullptr)
    {
        inOrderTraversal(root->left);
        cout << "ID: " << root->id_barang << ", Nama: " << root->nama << ", Harga: " << root->harga << endl;
        inOrderTraversal(root->right);
    }
}
Barang *bacaFile(Barang *root, ifstream &file)
{
    int id_barang;
    string  nama;
    float harga;

    while (file >> id_barang)
    {
        file.ignore();
        getline(file, nama, '\"'); 
        getline(file, nama, '\"'); 
        file >> harga;
        root = insertbst(root, id_barang, nama, harga);
    }
    return root;
}


int main()
{
    system("cls");
    Barang *root = nullptr;
    int menu, pilihan;
    int id_barang;
    string nama;
    float harga;
    ifstream rf("databarang.txt"); 
    root = bacaFile(root, rf);
    rf.close();
    ifstream checkFile("databarang.txt");
    bool sudahDimasukkan = false;

    if (checkFile.is_open()) {
        checkFile >> sudahDimasukkan;
        checkFile.close();
    }

    if (!sudahDimasukkan) {
        if (searchbst(root, 1) == nullptr) {
            root = insertbst(root, 1, "PocariSweat", 7500);
            
        }
        if (searchbst(root, 2) == nullptr) {
            root = insertbst(root, 2, "BangBang", 2500);
        }
        if (searchbst(root, 3) == nullptr) {
            root = insertbst(root, 3, "Aqua", 3000);
        }
        if (searchbst(root, 4) == nullptr) {
            root = insertbst(root, 4, "Nextar", 8000);
        }
        if (searchbst(root, 5) == nullptr) {
            root = insertbst(root, 5, "Beras 5Kg", 74000);
        }
        if (searchbst(root, 6) == nullptr) {
            root = insertbst(root, 6, "Le Minerale", 3500);
        }
        if (searchbst(root, 7) == nullptr) {
            root = insertbst(root, 7, "Casablanca", 28000);
        }
        if (searchbst(root, 8) == nullptr) {
            root = insertbst(root, 8, "SuperPell", 17000);
        }
        if (searchbst(root, 9) == nullptr) {
            root = insertbst(root, 9, "Indomie Goreng", 3500);
        }
        if (searchbst(root, 10) == nullptr) {
            root = insertbst(root, 10, "Oreo", 9000);
        
        }
        sudahDimasukkan = true;
        ofstream file("databarang.txt");
        file.close();
    }
    do
    {
        cout << "\nProgram Minimarket\n";
        cout << "1. BST\n";
        cout << "2. AVL\n";
        cout << "3. Keluar\n";
        cout << "Silahkan Pilih : ";
        cin >> menu;

        ofstream wf;
        ifstream rf;
        root = bacaFile(root, rf);
        wf.close();

        if (menu == 1)
        {
            if (root == nullptr)
                cout << "Tidak ada Barang tersedia.\n";
            else
            {
                cout<<"\n";
                cout << "List Barang:\n";
                inOrderTraversal(root);
                cout<<"\n-------------------------\n";
            }
            cout << "Menu BST \n";
            cout << "1. Tambah Barang\n";
            cout << "2. Hapus Barang\n";
            cout << "3. Cari Barang\n";
            cout << "4. Simpan Barang\n";
            cout << "Silahkan Masukkan Menu yang Anda pilih : ";
            cin >> pilihan;
            cout<<"\n";
            
            switch (pilihan)
            {
            case 1:
            {
                cout<<"ID Barang: ";
                cin>>id_barang;
                cout << "Nama Barang: ";
                cin.ignore();
                getline(cin, nama);
                cout << "Harga: ";
                cin >> harga;
                root = insertbst(root, id_barang, nama, harga);
                cout << "Barang telah berhasil ditambah\n";
                wf.open("databarang.txt", ios::trunc);
                simpanFile(root, wf);
                wf.close();    
                break;
            }
            case 2:
            {
             
                cout << "Masukkan ID Barang untuk dihapus: ";
                cin >> id_barang;
                root = hapusbst(root, id_barang, wf);
                cout << "Barang telah dihapus\n";
                wf.open("databarang.txt", ios::trunc);
                simpanFile(root, wf);
                wf.close();
                break;
            }
            case 3:
            {
                if (root == nullptr)
                {
                    cout << "Tidak ada barang yang tersedia.\n";
                }
                else
                {
                    int searchid;
                    cout << "Masukkan ID Barang yang ingin dicari: ";
                    cin>>searchid;
                    Barang *ketemu = searchbst(root, searchid);
                    if (ketemu != nullptr)
                    {
                        cout << "Barang ditemukan:\n ID: " << ketemu->id_barang <<"\n Nama: "<< ketemu->nama<<"\n Harga: " << ketemu->harga << endl;
                    }
                    else
                    {
                        cout << "Barang tidak ditemukan.\n";
                    }
                }
                break;
            }
            case 4:
            {
                wf.open("databarang.txt", ios::trunc);
                simpanFile(root, wf);
                wf.close();
                cout << "Barang Telah Disimpan";
                break;
            }

            default:
            {
                cout << "Pilih Sesuai Menu";
                break;
            }
            }
        }
        if (menu == 2)
        {
            if (root == nullptr)
                cout << "Tidak ada barang.\n";
            else
            {
                cout<<"\n";
                cout << "List Barang:\n";
                
                inOrderTraversal(root);
                cout<<"\n-------------------------\n";
            }
            cout << "Menu AVL \n";
            cout << "1. Tambah Barang\n";
            cout << "2. Hapus Barang\n";
            cout << "3. Cari Barang\n";
            cout << "4. Simpan Barang\n";
            cout << "Silahkan Masukkan Menu yang Anda pilih : ";
            cin >> pilihan;
            cout<<"\n";
            switch (pilihan)
            {
            case 1:
            {
                cout<<"ID Barang: ";
                cin>>id_barang;
                cout << "Nama Barang: ";
                cin.ignore();
                getline(cin, nama);
                cout << "Harga: ";
                cin >> harga;
                root = insertavl(root, id_barang, nama, harga);
                cout << "Barang telah berhasil ditambah\n";
                wf.open("databarang.txt", ios::trunc);
                simpanFile(root, wf);
                wf.close();    
                break;
            }
            case 2:
            {
                cout << "Masukkan ID Barang untuk dihapus: ";
                cin >> id_barang;
                root = hapusavl(root, id_barang);
                cout << "Barang telah dihapus\n";
                wf.open("databarang.txt", ios::trunc);
                simpanFile(root, wf);
                wf.close();
                break;
            }
            case 3:
            {
                if (root == nullptr)
                {
                    cout << "Tidak ada barang.\n";
                }
                else
                {

                    int searchid;
                    cout << "Masukkan ID Barang yang ingin dicari: ";
                    cin>>searchid;
                    Barang *ketemu = searchavl(root, searchid);

                    if (ketemu != nullptr)
                    {
                        cout << "Barang ditemukan:\n ID: " << ketemu->id_barang <<"\n Nama: "<< ketemu->nama<<"\n Harga: " << ketemu->harga << endl;
                    }
                    else
                    {
                        cout << "Barang tidak ditemukan.\n";
                    }
                }
                break;
            }
            case 4:
            {
                wf.open("databarang.txt", ios::trunc); 
                simpanFile(root, wf);
                wf.close();
                cout << "Barang telah disimpan\n";
                break;
            }

            default:
            {
                cout << "Pilih Sesuai Menu\n";
                break;
            }
            }
        }
    } while (menu != 3);

    cout << "Terimakasih Sudah Bermain dengan Program Kami\n";
    return 0;
}


