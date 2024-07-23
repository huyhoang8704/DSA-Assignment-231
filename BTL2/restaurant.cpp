#include "main.h"

#define vector_pair vector<pair<char,int>>
#define ll unsigned long long
#define queue_int queue<int>
#define vector_Node vector<Node*>
#define list_int list<int>
#define vector_string vector<string>
int MAXSIZE = 0;
class RESTAURANT;
class Gojo;
class Sukuna;
class HuffmanTree;

bool checkTypeChar(char a, char b){return (a >= 65 && a <= 90 && b >= 65 && b <= 90)|| (a >= 97 && a <= 122 && b >= 97 && b <= 122);}
vector_pair countFre(string str){
    vector_pair freq;
    for ( size_t i = 0; i < str.length(); i++){
        bool found = false;
        for ( size_t j = 0; j < freq.size(); j++){
            if (freq[j].first == str[i]){
                freq[j].second++;
                found = true;
                break;
            }
        }
        if (!found) freq.push_back(make_pair(str[i], 1));
        
    }
    return freq;
}
void ceasar(vector_pair &freq){
    for( size_t i=0;i<freq.size();i++){
        if(isupper(freq[i].first))   freq[i].first = (freq[i].first -65 + freq[i].second) % 26 + 65;
        else                         freq[i].first = (freq[i].first -97 + freq[i].second) % 26 + 97;
    }
}
string accessnameCeasar(string s, const vector_pair& freq) {
    for (size_t i = 0; i < s.size(); i++) {
        for ( size_t j = 0; j < freq.size(); j++) {
            if (s[i] == freq[j].first) {
                char base = isupper(s[i]) ? 'A' : 'a';
                s[i] = (s[i] - base + freq[j].second) % 26 + base;
                break; // Thêm break để ngăn chặn việc so sánh với các phần tử còn lại trong freq
            }
        }
    }
    return s;
}
int binToDec(const std::string& binaryString) {
    bitset<64> bits(binaryString);
    return static_cast<int>(bits.to_ullong());
}
class HuffmanTree{
	class Node;
private:
	class Node{
	public:
		int wgt;
		char c;
		Node* left;
		Node* right;
		friend class HuffmanTree;
	public:
		Node(int wgt, char c = '\0',Node* left = nullptr, Node* right = nullptr ):  wgt(wgt), c(c), left(left), right(right) {}
		bool isChar() const { return c != '\0'; }
	};
int height(Node *node){if(!node) return 0; return 1 + max(height(node->left),height(node->right));}
Node* rotateLeft(Node* root){
    Node* temp = root->right;
    root->right = temp->left;
    temp->left = root;
    return temp;
}
Node* rotateRight(Node* root){
    Node* temp = root->left;
    root->left = temp->right;
    temp->right = root;
    return temp;   
}
int getBalance(Node * node){
    if (node == nullptr) return 0;
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    int balance = leftHeight - rightHeight;
    return balance;
}
Node* balanceNode(Node* node, int& count) {
    int balance = getBalance(node);
    //* LL
    if(balance > 1 && getBalance(node->left) >= 0 && count < 3){
        count++;
        return rotateRight(node);
    }
    //*RR
    if(balance <-1 && getBalance(node->right) <=0 && count < 3 ){
        count++;
        return rotateLeft(node);
    }
    //* RoL , LR
    if(balance > 1 && getBalance(node ->left) < 0 && count < 3){
        node->left = rotateLeft(node->left);
        count ++;
        return rotateRight(node);
    }
    //* LoR
    if(balance < -1 && getBalance(node ->right) > 0 && count < 3){
        node->right = rotateRight(node->right);
        count ++;
        return rotateLeft(node);
    }
    return node;  // Trường hợp đã cân bằng
}
void balanceHuffman(Node* &temp, int& count){
    if(!temp || count >=3) return;
    temp = balanceNode(temp,count);
    balanceHuffman(temp->left,count);
    balanceHuffman(temp->right,count);
}
private:
	Node* root;
public:
	~HuffmanTree(){delete_HuffmanTree(root);}
	void delete_HuffmanTree(Node* temp){
		if(temp){
			delete_HuffmanTree(temp->left);
			delete_HuffmanTree(temp->right);
			delete temp;
			temp = nullptr;
		}
	}
	void print_inorder(Node* temp){
		if(!temp) return;
		print_inorder(temp->left);
		if(temp->c == '\0') cout << temp->wgt << "\n";
		else cout << temp->c << "\n";
		print_inorder(temp->right);
	}
	void HAND(){print_inorder(root);}
	void HuffmanCode(vector_string& code, Node* node, string s = ""){
		if(!node) return;
    	if(node ->isChar()) code[node->c]  = s ;
    	HuffmanCode(code,node->left , s +"0");
    	HuffmanCode(code,node->right , s +"1");
}
//*1
vector_pair countFrequencyCeasar(string& name)
{
    vector_pair freq = countFre(name);
	if(freq.size() < 3) return{};
    //string nameCeasar = accessnameCeasar(name,freq_prev);
    ceasar(freq);
	// cộng dồn
	vector_pair freq_ceasar;
	for( size_t i=0;i<freq.size();i++){
		bool found = false;
		for( size_t j=0;j<freq_ceasar.size();j++){
			if(freq[i].first == freq_ceasar[j].first){
				freq_ceasar[j].second += freq[i].second;
				found = true;
				break;
			}
		}
		if(!found) freq_ceasar.push_back(freq[i]);
	}
	// sắp xếp tần suất
	sort(freq_ceasar.begin(), freq_ceasar.end(), [=](const pair<char, int> &a, const pair<char, int> &b) -> bool
		{
            if (a.second > b.second) return true;
    		if (a.second == b.second) {
        		if (checkTypeChar(a.first,b.first))
            	return a.first > b.first;
        		else return a.first < b.first;
			}
   			return false; 
		});
    return freq_ceasar;
}
//*2
Node* CreateHuffman(vector_pair freq){
    vector_Node node;
    for(pair<char,int> & pair : freq){
        Node* temp = new Node(pair.second, pair.first);
        node.push_back(temp);
    }
    while(node.size() > 1){
        Node *left = node.back(); node.pop_back(); // nhỏ thứ nhất
        Node *right = node.back(); node.pop_back(); // nhỏ thứ hai

        Node* temp = new Node(left->wgt + right->wgt, '\0',left,right);
        int cnt = 0;  // cnt <=3
        balanceHuffman(temp,cnt);
        balanceHuffman(temp,cnt);
        balanceHuffman(temp,cnt);
        if(node.size() == 0 || node.back() ->wgt > temp->wgt) {node.push_back(temp);}
        else {
            for( size_t i=0;i<node.size();i++){
                if(temp ->wgt >= node[i]->wgt){
                    node.insert(node.begin()+i,temp);
                    break;
                }
            }
        }
    }
    return node[0];
}
//*3
int Huffman3(Node * root,string nameCaesar){
    if(root->left == nullptr && root->right == nullptr) return 0;
    vector_string code(256, "");
    HuffmanCode(code, root);
    string bin = "";
    for( size_t i=0;i<=nameCaesar.size()-1;i++) bin += code[nameCaesar[i]];
        
    if (bin.size() > 10) bin = bin.substr(bin.size() -10);
    reverse(bin.begin(),bin.end());
    
    int result = 0;
    result = binToDec(bin);
    return result;
}
int main(string name){
	vector_pair freq_previous = countFre(name);
	if (freq_previous.size() < 3) return -1;

	vector_pair freq = countFrequencyCeasar(name);
	if(freq.size() == 0 ) return -1;
 	this->delete_HuffmanTree(root);
	root = CreateHuffman(freq);
	if(!root->left && !root->right ) return 0; // trường hợp có 1 node

	int result = 0;
	string nameCeasar = accessnameCeasar(name , freq_previous);
	result = Huffman3(root , nameCeasar);
	return result;
}
};

class Gojo{
	class BinarySearchTree;
private:
	vector<BinarySearchTree> area; 
public:
	Gojo():area(MAXSIZE + 1){} 
	void insertCustomerToArea(int n)  {int ID = n % MAXSIZE + 1;  area[ID].insert_Customer(n);}

	void KOKUSEN(){for( size_t i = 1; i <= static_cast<size_t>( MAXSIZE) ; i++) area[i].Xoa_kokusen();}

	void print_BST_Inorder(int n){if(n <= 0 || n > MAXSIZE) return; area[n].print();}
private:
	class BinarySearchTree{
		class Customer;
	private:
		class Customer{
		private:
			int result;
			Customer* left;
			Customer* right;
			friend class BinarySearchTree;
		public:
			Customer(int n) : result(n), left(nullptr), right(nullptr) {}
		};
	private:
		Customer* root;	
		queue_int time; 
	public:
		BinarySearchTree():root(nullptr){}
		~BinarySearchTree(){
			while(!time.empty()){
				int custumer = time.front();
				time.pop();
				root = remove_Customer(root , custumer);
			}
		}
		void print_inorder(Customer* temp){
			if(temp){
				print_inorder(temp->left);
				cout << temp->result << "\n";
				print_inorder(temp->right);
			}
		}
		void print(){print_inorder(root);}
		int size(){return time.size();}
		void insertRec(Customer *&node , int result){
			if (node == nullptr){
				node = new Customer(result);
			    time.push(result);
                return;
			}
			else {
				if(result < node->result)  insertRec(node->left, result);
				else  insertRec(node->right, result);
			}
		}
		void insert_Customer(int n){insertRec(root,n); }
		Customer* remove_Customer(Customer* node,int result)
		{	
			if(node == nullptr) return node;
			if( result < node->result) node->left = remove_Customer(node->left, result);
			else if (result > node->result) node->right = remove_Customer(node->right, result);
			else {
				Customer* nodeDelete = node;
				if( !node->left  && !node->right )     node = nullptr;
				else if (node->left == nullptr)        node = node->right; 
				else if (node->right == nullptr)       node = node->left;
				else{
					Customer* temp = node -> right;
					while(temp->left != nullptr) temp = temp->left; // lấy node nhỏ nhất của cây con bên phải
					swap(node->result, temp->result);
					node->right = remove_Customer(node->right, result);
					return node;
				}
				delete nodeDelete;
				nodeDelete = nullptr;
			}  
			return node;
 		}


		int CountNodeWithoutRecur(Customer* root) {
    if (root == NULL) {
        return 0;
    }

    int count = 0;
    std::stack<Customer*> nodeStack;
    nodeStack.push(root);

    while (!nodeStack.empty()) {
        Customer* current = nodeStack.top();
        nodeStack.pop();
        count++;

        if (current->right != NULL) {
            nodeStack.push(current->right);
        }

        if (current->left != NULL) {
            nodeStack.push(current->left);
        }
    }

    return count;
}
		
		ll C (int x, int n){
			ll Cxn = 1;
			for( size_t i= 1 ; i<=static_cast<size_t>(x) ;i++){
				Cxn *= (n-x+i);
				Cxn /= i;
			}
			return Cxn;
		}
		ll solu(Customer* node)
		{
			if(node == NULL) return 1;
			ll left = CountNodeWithoutRecur(node->left);
			ll right = CountNodeWithoutRecur(node->right);
			return (solu(node->left) * solu(node ->right) * C(left , left + right)) % MAXSIZE; 
		}
		void Xoa_kokusen(){
			if(this->size() == 0) return; 
			ll Y = solu(root);

			while(Y != 0 && !time.empty()){
				int temp = time.front();			
				time.pop(); 						
				root = remove_Customer(root ,temp);	
				Y --;
			}
		}
	};
};
class Sukuna{
	class KhuVuc;
private:
	class KhuVuc{
	private:
		int ID;				
		list_int list_customer; 		
		friend class Sukuna;
	public:
		KhuVuc(int ID) : ID(ID) {}
		int size() { return list_customer.size(); }
		int size_customer() const { return list_customer.size(); }
		void insert_customer(int n){list_customer.push_front(n);}
		void print_list_remove(int n){while(n != 0 && !list_customer.empty()){cout << list_customer.back() << "-" <<ID << "\n"; list_customer.pop_back(); n--;}}
		void print(int n){
			list_int::iterator it;
			for(it = list_customer.begin(); n > 0 && it != list_customer.end(); ++it, --n){cout << ID << "-" << *it  << "\n";}
		}
	};
private:
	vector<KhuVuc*> area;  
	list<KhuVuc* > recently; 		 
private:
	int Compare(int index1, int index2)
	{
		if (index1 == static_cast<int>(area.size()) - 1) return index1;
		if (area[index1]->size_customer() > area[index2] -> size_customer() ) return index2;
		else if (area[index1]->size_customer() == area[index2] -> size_customer()){
			auto a = find(recently.begin(),recently.end(),area[index1]);
			auto b  = find(recently.begin(),recently.end(),area[index2]);
			if (distance(recently.begin(),a) < distance(recently.begin(),b)) return index2;
			else return	index1;
		}
		else return index1;
	}
	bool check(int parent , int child){   // parent > child thì là true
		auto a = find(recently.begin(),recently.end(),area[parent]);
		auto b  = find(recently.begin(),recently.end(),area[child]);
		if (distance(recently.begin(),a) < distance(recently.begin(),b)) return true;			
		return false;
	}

	void Redown(int pos)
	{
		while (static_cast<size_t>(pos) < area.size() / 2){
			int k = pos;
			int left = pos * 2 + 1;
			int right = pos * 2 + 2;
			int child_min = Compare(left,right);  // tìm thằng nhỏ nhất giữa 2 thằng
			if (area[child_min]->size_customer() < area[pos]->size_customer()) pos = child_min;
			else if (area[child_min]->size_customer() == area[pos]->size_customer()){
				if(this->check(pos,child_min)) pos = child_min; // trường hợp nếu thằng child_min đến sau thì gán cho index
			}
			if (pos != left && pos != right) return;
			else swap(area[k],area[pos]); 
		}
	}
	void ReUp(int pos) {
		int parent_index = (pos-1)/2;
		if (pos == 0 || area[parent_index] ->size_customer() <= area[pos]->size_customer()) return;
		swap(area[pos],area[parent_index]);
		ReUp(parent_index);
	}

	void AreaRecently(KhuVuc* temp) {	
		auto it = find(recently.begin(),recently.end(),temp);
		if(it != recently.end()) recently.splice(recently.begin(),recently,it);
		else recently.push_front(temp);
	}


    void removeNode(KhuVuc* &node)
	{
		for(auto i:recently){
			if(i == node){
				recently.remove(node);
			}
		}
	}
public:
	Sukuna(){}
	~Sukuna(){
		for( size_t i = 0; i < area.size(); i++) {
			delete area[i];
			area[i] = nullptr;
		}
	}
	void print_Preorder(int pos, int n){	
		if (pos >= static_cast<int>(this->area.size()) || n <= 0) return;
		this->area[pos]->print(n);
		print_Preorder(pos * 2 + 1, n);
		print_Preorder(pos * 2 + 2, n);
	}
	void CLEAVE(int n){print_Preorder(0, n);}
	void insertArea(int n){
		int ID = n % MAXSIZE + 1;
		int pos = -1; 
		for( size_t i=0;i<area.size();i++) if(area[i]->ID == ID) pos = i;
		if(pos == -1){
			area.push_back(new KhuVuc(ID));
			pos = area.size() - 1;
			area[pos]->insert_customer(n);
			this->AreaRecently(area[pos]);
			this->ReUp(pos);
		}
		else {
			area[pos]->insert_customer(n);
			this->AreaRecently(area[pos]);
			this->Redown(pos);
		} 
	}
		void KEITEIKEN(int NUM) {
		if(area.size() <= 0) return;
		vector<KhuVuc*> temp(area.begin(),area.end());
		queue <KhuVuc*> listAreaDelete;
		for( size_t i=0; area.size() && i <static_cast<size_t>(NUM) ;i++){
			KhuVuc *temp = area[0];
			swap(area[0] , area[area.size()-1]);
			area.pop_back();
			this->Redown(0);
			listAreaDelete.push(temp);
		}
		area = temp; 
		while(listAreaDelete.size()){
			KhuVuc* nodeDelete = listAreaDelete.front();
			listAreaDelete.pop();
			nodeDelete->print_list_remove(NUM); // xóa và in ra customer theo cơ chế FIFO
			int pos = 0;
			while(area[pos] !=  nodeDelete) pos++;
			if(nodeDelete->size() == 0){
				swap(area[pos], area[area.size() - 1]);
				this->removeNode(area[area.size() - 1]);
				delete area[area.size() - 1];
				area[area.size() - 1] = nullptr;
				area.pop_back();
			}
			this->Redown(pos);
		}		
 	}
};
class RESTAURANT{
private:
	HuffmanTree customer;
	Gojo gojo;
	Sukuna sukuna;
public:
	void LAPSE(string name){
		int result = customer.main(name);
		//solution << "result = " << result << endl;
		if(result == -1) return;
		if(result % 2 == 1) gojo.insertCustomerToArea(result);
		else sukuna.insertArea(result);
	}	
	void KOKUSEN(){gojo.KOKUSEN();}
	void LIMITLESS(int num){gojo.print_BST_Inorder(num);}
	void KEITEIKEN(int num){sukuna.KEITEIKEN(num);}
	void CLEAVE(int num){sukuna.CLEAVE(num);}
	void HAND(){customer.HAND();}
};
void simulate(string filename){
	ifstream ss(filename);
	string str, name;
	int num;
	ss >> str; ss >> MAXSIZE; 
	RESTAURANT* r = new RESTAURANT();
	while (ss >> str)
	{
		if (str == "LAPSE") {ss >> name;r->LAPSE(name);}
		else if (str == "KOKUSEN") r->KOKUSEN();
		else if (str == "KEITEIKEN") {ss >> num;r->KEITEIKEN(num);}
		else if (str == "HAND") r->HAND();		
		else if (str == "LIMITLESS") {ss >> num;r->LIMITLESS(num);}		
		else if (str == "CLEAVE") {ss >> num;r->CLEAVE(num);}				
	}
	delete r;
}

