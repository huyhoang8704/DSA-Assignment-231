#include "main.h"


class imp_res : public Restaurant
{
private:
	customer *cusX;  // ds lk doi vong
	int sizeCus;
	customer *cusQueue;  // ds liên kết đôi vòng
	int sizeCusQueue;
public:
	class cusTime{ // danh sach lk doi
	public:
		customer *data;
		cusTime *next;
		cusTime *prev;
		bool inDesk; // xem thử có trong bàn hay không
	public:
		cusTime(customer *data, bool inDesk = false , cusTime *next = nullptr , cusTime *prev = nullptr) :data(data),next(next), prev(prev), inDesk(inDesk) {}
		~cusTime(){delete data;}
		// 1 customerTime quản lí customer delete customerTime sẽ delete customer luôn
	};
private:
	cusTime *cusTimeHead;
	cusTime *cusTimeTail;
public:	
		imp_res() {
			cusX = cusQueue = nullptr;
			sizeCus = sizeCusQueue = 0;
			cusTimeHead = cusTimeTail = nullptr;
		};
		~imp_res(){  
		while(cusTimeHead) // hàm hủy khách time
		{
			cusTime* temp = cusTimeHead;	 
			cusTimeHead = cusTimeHead->next;
			delete temp;
		}
	}
		//* DOMAIN
		// print ra khách hàng đến gần nhất đến lâu nhất trong bàn
		void print_DOMAIN(cusTime* head){
			if(head != nullptr){
				print_DOMAIN(head->next);
				head->data->print();
			}
		}
        void addCusTime(cusTime *cus){
			if (!cusTimeHead){
				cusTimeHead = cusTimeTail = cus;
			}
			else {
				cusTimeTail->next = cus;
				cus->prev = cusTimeTail;
				cusTimeTail = cus;
			}
		}
		void addCusQueue(customer *cus){
			if(!sizeCusQueue){
				cusQueue = cus;
				cusQueue ->next = cusQueue ->prev = cus;
			}
			else {
				cus -> next = cusQueue;
				cus -> prev = cusQueue->prev;
				cusQueue->prev = cus;
				cus ->prev -> next = cus;
			}
			this->sizeCusQueue++;
		}
		void RED(string name, int energy)
		{
			if(energy == 0 || sizeCus + sizeCusQueue >= MAXSIZE * 2) return;
			// check xem khách hàng nào trùng tên thì đuổi về
			else {
				for(cusTime *k = cusTimeHead ; k!=nullptr ; k= k->next) if(k->data ->name == name) return;
			}
			// số lượng khách trong bàn full -> thêm vào hàng chờ , customerTime
			customer *cus = new customer (name, energy, nullptr, nullptr); // new customer
			if (this->sizeCus == MAXSIZE) {
				addCusQueue(cus);
				addCusTime(new cusTime(cus , false));
				return;
			}
			// khách hàng đầu tiên
			else if(sizeCus == 0) {
				cusX = cus;
				cusX -> prev = cusX->next = cusX;
				this->sizeCus++;
				addCusTime(new cusTime (cus,true)); // Time
				return; 
			}
			// add customer bình thường 
			else {
				if (sizeCus >= MAXSIZE/2){
				customer* temp = cusX;
				for(int i = 0; i < sizeCus; i++, temp = temp->next) if(abs(energy - temp->energy) > abs(energy - cusX->energy)) cusX = temp;
				}
				//*  trường hợp chèn theo chiều kim đồng hồ
				if(energy >= cusX->energy){
					cus->next = cusX->next;
			        cus->prev = cusX;
			        cusX->next =cus->next->prev = cus;
					cusX = cus;
			        sizeCus ++;
			    }
				//*  trương hợp chèn ngược chiều kim đồng hồ
				else{
					cus->next = cusX;
			        cus->prev = cusX->prev;
			        cusX->prev = cus->prev->next = cus;
			        cusX = cus;
			        sizeCus++;
		        }
			}
			addCusTime(new cusTime(cus,true));
			return;
		}
		
		void deleteCusInDesk(cusTime* cusTimeDelete){ 
			if(sizeCus == 1) {
				delete cusX;
				cusX = nullptr;
			}
			else {
				customer* cusDelete = cusTimeDelete -> data;
				if(cusDelete->energy > 0) cusX = cusDelete->next;
			    else                      cusX = cusDelete ->prev;
				
				// cập nhật tại khách hàng phía trước và khách hàng phía sau
				cusDelete -> next ->prev  = cusDelete ->prev;
				cusDelete->prev ->next = cusDelete->next;
				// cập nhật lại khách hàng x 
			}
			delete cusTimeDelete;
			sizeCus--;
		}
		void deleteCustomerInQueue(cusTime * cusTimedelete){
			if(sizeCusQueue== 1) {
				delete cusQueue;
				cusQueue = nullptr;
			}
		    else{
			customer* cusDelete = cusTimedelete->data;
			//* cập nhật xóa
			cusDelete->next->prev = cusDelete->prev;
			cusDelete->prev->next = cusDelete->next;

			//* cập nhật customerQueue
			if(cusDelete == cusQueue) cusQueue = cusQueue->next;
		    }
			delete cusTimedelete;
		    sizeCusQueue--;
		}
		void BLUE(int num){
			if (num > MAXSIZE) num = MAXSIZE;
			if (sizeCus == 0) return;
			//*  số lượng khách hàng bị đuổi
			for(int i = 1; i <= num && sizeCus !=0 ; i++){
				//*  Tìm khách chuẩn bị đuổi và xóa khỏi danh sách Time
			    cusTime* cusTimeDelete = cusTimeHead;
			    for(int i = 0; i < sizeCus + sizeCusQueue && !cusTimeDelete->inDesk; i++, cusTimeDelete = cusTimeDelete->next);
				//^ Xóa khỏi danh sách hàng customerTime
		        if(sizeCus + sizeCusQueue == 1) cusTimeHead = cusTimeTail = nullptr;
			    else if(cusTimeDelete == cusTimeHead){
				    cusTimeHead->next->prev = nullptr;
				    cusTimeHead = cusTimeHead->next;
			    }
			    else if(cusTimeDelete == cusTimeTail){    
				    cusTimeTail->prev->next = nullptr;
				    cusTimeTail = cusTimeTail->prev;
			    }
			    else{
				    cusTimeDelete->next->prev = cusTimeDelete->prev;
				    cusTimeDelete->prev->next = cusTimeDelete->next;
			    }
			    //* Bước 1.2
			    this->deleteCusInDesk(cusTimeDelete);
		        }
			add_to_desk();
		}
		void add_to_desk(){
			while(sizeCusQueue != 0 && sizeCus != MAXSIZE)
			{
				customer* Cusnew = cusQueue; // đầu hàng đợi 
				cusTime* temp = cusTimeHead; 
				for(int i = 1; i <= sizeCus + sizeCusQueue; i++, temp = temp->next){
					if (temp ->data -> name == Cusnew -> name){
						temp->inDesk = true;
						break;
					}
				}
				//*  xóa khách hàng trong hàng chờ 
				if(sizeCusQueue == 1) cusQueue = nullptr;
				else{
					cusQueue->prev->next = cusQueue->next;
				    cusQueue->next->prev = cusQueue->prev;
				    cusQueue = cusQueue->next;
				}
				sizeCusQueue --;
				//* thêm khách hàng vào bàn
				if(sizeCus == 0) Cusnew->next = Cusnew->prev = Cusnew;
			    else{
				    if(sizeCus >= MAXSIZE / 2){
						customer* temp = cusX;
					    for(int i = 0; i < sizeCus; i++, temp = temp->next){
							if(abs(Cusnew->energy - temp->energy) > abs(Cusnew->energy - cusX->energy)) 
							    cusX = temp;
					    }
				    }
				    if(Cusnew->energy >= cusX->energy){
						Cusnew->next = cusX->next;
					    Cusnew->prev = cusX;
					    cusX->next = Cusnew->next->prev = Cusnew;
				    }
				    else{
						Cusnew->next = cusX;
					    Cusnew->prev = cusX->prev;
					    cusX->prev = Cusnew->prev->next = Cusnew;
				    }
			    }
			    cusX = Cusnew;
			    sizeCus ++;
			    }
		}


	customer* getIndex(customer* x, int index)
	{
		for(int i = 1; i <= index; i++) x = x->next;
		return x;
	}

	bool compare(customer* a, customer* b){
		if(abs(a->energy) > abs(b->energy)) return true;
		else if(abs(a->energy) < abs(b->energy)) return false;
		for (cusTime * temp = cusTimeTail ; temp !=nullptr ; temp = temp->prev){
			if(temp->data == a) return false;
			else if(temp->data == b) return true;
		}
		return false;
	}

	void ShellSort(customer* head, int n, int gap, int &N) {
		for(int i = gap; i < n; i += gap)
		{
			for(int j = i; j >=gap; j -= gap)
			{
				customer* b = this->getIndex(head, j - gap);
				customer* a = this->getIndex(b, gap);

				if(compare(a,b)){
					this->swapAddress(a,b);
					if(b == cusQueue) cusQueue = a;
					if(head == b) head = a;
					N++;
				}
				else break;
			}
		}
	}		
void PURPLE()
{
	if(sizeCusQueue <= 1) return;
	customer* temp = cusQueue;
	customer* max = cusQueue;
	for(int i=1 ; i<=sizeCusQueue ;i++){
		if (abs(temp ->energy) >= abs(max->energy)){
			max = temp;
	    }
	    temp = temp->next;
	}
	int n = 1; // Tính số phần tử ShellSort
	for (customer * k = cusQueue ; k!= max ; k = k->next) n++;

	int N = 0;
	for(int i = n / 2; i > 2; i /= 2)
	{
		for(int j = 0; j < i; j++)
		{
			customer* head = this->getIndex(cusQueue, j);
			ShellSort(head, n - j, i, N);
		}
	}
	ShellSort(cusQueue, n, 1, N);
	this->BLUE(N % MAXSIZE);
}
        //* REVERSAL
		customer* findHead(customer* temp){
			for(int i = 1; i <= sizeCus; i++){
				if(temp->energy  > 0) break;
			    temp = temp ->prev;
		    }
		    return temp;
		}
	    customer* findTail(customer* temp){
		    for(int i = 1; i <= sizeCus; i++){
				if(temp->energy  > 0) break;
			    temp = temp -> next;
		    }
		    return temp;
		}

		void swapAddress(customer * a, customer *b){
			customer* temp = new customer();
		    // thế temp vào a
			temp -> next = a -> next;
			temp -> prev = a -> prev;
			a -> next -> prev = a -> prev -> next = temp;
			// đổi a với b
			a -> next = b -> next;
			a -> prev = b -> prev;
			b -> next -> prev = b -> prev -> next = a;
			// đổi b với temp
			b -> next = temp -> next;
			b -> prev = temp -> prev;
			temp->next->prev = temp->prev->next = b ;
		    delete temp;
		}
			
		void REVERSAL() 
		{
			if(sizeCus <= 1) return;			

			customer *head_Thuat_Si = this->findHead(cusX);
			customer *tail_Thuat_Si = this->findTail(cusX->next);

			customer* head = nullptr; 
			customer* tail = nullptr;
			customer * p= cusX;

		//* Đảo oán linh  
			//* : tìm head bằng cách duyệt theo ngược chiều kim đồng hồ từ khách hàng X tìm oán linh
			head = cusX;
			for(int i=1 ;i<=sizeCus;i++){
				if (head->energy <0) break;
				head = head->prev;
			}

			//* : tìm tail bằng cách duyệt theo chiều kim đồng hồ từ khách hàng trước X tìm oán linh
			tail = cusX->next; // NHƯNG PHẢI KHÁC khách hàng x duyệt lần đầu
			for(int i = 1; i<= sizeCus; i++){
				if (tail ->energy <0) break;
				tail = tail->next;
			}

			// chỉ có 2 khách hàng là oán linh trở lên mới đảo
			if(head->energy < 0 && head != tail){
				for(int i = 1; i <= sizeCus / 2; i++)
				{
					this->swapAddress(head, tail); 
					customer* tmp = head;
					head = tail;
					tail = tmp;
					customer* temp = head->prev;

					// tìm head tiếp theo
					for(int i = 1; i <= sizeCus; i++)
					{
						if (temp ->energy <0){ 
							head = temp;
							break;
						}
						temp= temp->prev;

					}
					// xét xem thằng head mới đó có phải là tail không , nếu bằng thì dừng
					if(head == tail) break;		

					// tìm tail tiếp theo
					temp = tail->next;
					for(int i = 0; i < sizeCus ; i++)
					{
						if(temp ->energy <0){
							tail = temp;
							break;
						}
						temp = temp->next;

					}	
					// nếu thằng tail mới mà giống thằng head mới đổi không , nếu giống thì dừng
					if(head == tail) break;	
				}
			}

		//* Đảo thuật sĩ
			head = head_Thuat_Si;
			for(int i=1 ;i<=sizeCus;i++)
			{
				if (head->energy >0) break;
				head = head ->prev;
			}
			tail = tail_Thuat_Si; 
			for(int i = 1; i<= sizeCus; i++)
			{
				if (tail ->energy >0) break;
				tail = tail->next;
			}

			// chỉ có 2 khách hàng là thuật sư trở lên mới đảo
			if(head->energy > 0 && head != tail){
				for(int i = 1; i <= sizeCus / 2; i++)
				{
					this->swapAddress(head, tail); 
					customer* tmp = head;
					head = tail;
					tail = tmp;
					customer* temp = head->prev;
					// tìm head tiếp theo
					for(int i = 1; i <= sizeCus; i++)
					{
						if (temp ->energy >0){ 
							head = temp;
							break;
						}
						temp= temp->prev;
					}
					if(head == tail) break;	// nếu còn hai thằng thì thằng trước đó sẽ = thằng tail
					// tìm tail tiếp theo
					temp = tail->next;
					for(int i = 1; i <= sizeCus ; i++)
					{
						if(temp ->energy >0){
							tail = temp ;
							break ;
						}
						temp = temp->next;
					}	
					if(head == tail) break;	
				}
			}
		}
		
		
		void UNLIMITED_VOID()
		{
			//cout<<"UNLIMITED_VOID"<<endl;
			if(sizeCus <= 3) return;

			int MINTOTAL = 2147483646;
			int k=0;
			customer* head = nullptr, * tail = nullptr; 

			customer* tempi = cusX; // khách hàng i
			for(int i = 1; i <= sizeCus; i++)
			{
				int total = 0;  // tổng từ chạy i -> i + sizeCustomerInDesk
				customer* tempj = tempi; // khách hàng j
				for(int j = 1; j <= sizeCus; j++)
				{
					total += tempj->energy;
					if ((MINTOTAL > total || MINTOTAL == total && j>=k) && j >= 4){
						MINTOTAL = total;
						k = j;
						head = tempi;
						tail = tempj;
					}
					tempj = tempj->next;
				}
				tempi = tempi->next;
			}
			customer *Min_list = head;
			customer * temp = head;
			int cnt=1;
			for(int i=0 ;i<k;i++){
				if(Min_list->energy > temp->energy){
					Min_list = temp;
					cnt = i+1;
				}
				temp = temp->next;
			}
			for(int i=1;i<=k-cnt+1;i++){
				Min_list->print();
				Min_list = Min_list ->next;
			}
			for(int i=1;i<=cnt-1;i++){
				head->print();
				head = head -> next;
			}
		}
			
		void DOMAIN_EXPANSION(){
			//cout<<"DOMAIN"<<endl;
			if (sizeCus + sizeCusQueue == 0 ) return;
			//* Bước 1 tính tổng của thuật sư và oán linh trong bàn ăn và hàng chờ
			int total_Thuat_Si = 0; // Tổng thuật sư
			int total_Oan_Linh = 0; // Tổng thuật sư + oán linh 
			int count_Thuat_Si = 0;
			int count_Oan_Linh = 0;

			// Tính tổng energy của customer đang có mặt tại nhà hàng
			for(cusTime *temp = cusTimeHead; temp !=nullptr ; temp = temp->next){
				if (temp ->data ->energy > 0) total_Thuat_Si += temp->data->energy;
				else                          total_Oan_Linh += temp ->data ->energy;
			}
			//* Trong hàng chờ + bàn ăn vào danh sách
			cusTime* Thuat_Si_Tail = nullptr; 
			cusTime* Oan_Linh_Tail = nullptr; 
			cusTime* Thuat_Si_Head = nullptr; 
			cusTime* Oan_Linh_Head = nullptr; 
			cusTime *tempTime = cusTimeHead;
			for(int i = 1; i <= sizeCusQueue + sizeCus; i++)
			{
				// thuật sư
				if(tempTime->data->energy > 0)
				{
					// nhớ xet TH WizardQueueHead == nullptr
					if (Thuat_Si_Head == nullptr) Thuat_Si_Head = Thuat_Si_Tail = tempTime;
					else{
						Thuat_Si_Tail -> next = tempTime;
						tempTime->prev = Thuat_Si_Tail;
						Thuat_Si_Tail = tempTime;
					}
					count_Thuat_Si++;
				}
				// oán linh
				else
				{	
					if (Oan_Linh_Head == nullptr) Oan_Linh_Head = Oan_Linh_Tail = tempTime;
					else{
						Oan_Linh_Tail->next = tempTime;
						tempTime->prev = Oan_Linh_Tail;
						Oan_Linh_Tail = tempTime;
					}
					count_Oan_Linh++;
				}
				tempTime = tempTime->next;
			}
			// cập nhật TimeHead, TimeTail
			if(Oan_Linh_Tail != nullptr) Oan_Linh_Tail->next = nullptr;
			if(Thuat_Si_Tail != nullptr) Thuat_Si_Tail->next = nullptr;
			
			//* xóa 
			if(total_Thuat_Si >= abs(total_Oan_Linh))  // thuật sư >= | thuật sự + oán linh |
			{
				print_DOMAIN(Oan_Linh_Head);
				while(count_Oan_Linh--){
					cusTime* cusTimeDelete = Oan_Linh_Head;
					if (cusTimeDelete->inDesk)       this->deleteCusInDesk(cusTimeDelete);
					else if (!cusTimeDelete->inDesk) this->deleteCustomerInQueue(cusTimeDelete);
					Oan_Linh_Head = Oan_Linh_Head->next;
				}
				//* cập nhật CustomerTimeHead và CustomerTimeTail cho thuật sĩ
				cusTimeHead = Thuat_Si_Head;
				cusTimeTail = Thuat_Si_Tail;
			}
			else{                                  // oán lính > thuật sĩ                                
				print_DOMAIN(Thuat_Si_Head);
				while(count_Thuat_Si--){
					cusTime* cusTimeDelete = Thuat_Si_Head;
					if (cusTimeDelete->inDesk)       this->deleteCusInDesk(cusTimeDelete);
					else if (!cusTimeDelete->inDesk) this->deleteCustomerInQueue(cusTimeDelete);
					Thuat_Si_Head = Thuat_Si_Head->next;
				}
				// như trên
				cusTimeHead = Oan_Linh_Head;
				cusTimeTail = Oan_Linh_Tail;
			} 
			add_to_desk();
		}
		void LIGHT(int num)
		{
			//cout<<"================================================================"<<endl;
			if(num == 0){
				customer* temp = cusQueue;
				for(int i=0;i<sizeCusQueue;i++){
					temp->print();
					temp=temp->next;
				}
			}
			else if (num > 0){	
				customer* temp = cusX;
				for(int i=0;i<sizeCus;i++){
					temp->print();
					temp=temp->next;
				}
			}
			else if (num < 0){
				customer* temp = cusX;
				for(int i=0;i<sizeCus;i++){
					temp->print();
					temp=temp->prev;
				}
			}
		}
		void InTime(){
			cout<<endl;
			for(cusTime *k = cusTimeHead;k!=nullptr;k=k->next) cout<<k->data->name<<"-"<<k->data->energy<<endl;
			cout<<endl;
		}
		
};