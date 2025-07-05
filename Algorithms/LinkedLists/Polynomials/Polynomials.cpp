#include <iostream>

using namespace std;

struct Node {
	float coeff;
	int expo;
	struct Node* link;
};

struct Node* insert(struct Node* head, float data, int exp) {

	struct Node* temp;
	struct Node* newP = new Node;
	newP->coeff = data;
	newP->expo = exp;
	newP->link = nullptr;

	if (head == nullptr || exp > head->expo) {
		newP->link = head;
		head = newP;
	}
	else {
		temp = head;
		while (temp->link != nullptr && temp->link->expo > exp) {
			temp = temp->link;
		}
		newP->link = temp->link;
		temp->link = newP;
	}
	return head;
}

struct Node* create(struct Node* head) {
	int n;
	int i;
	float coeff;
	int expo;

	cout << "Enter the number of terms: " << endl;
	cin >> n;

	for (i = 0; i < n; i++) {
		cout << "Enter the coefficient for term " << i + 1 << endl;
		cin >> coeff;

		cout << "Enter the exponent for term" << i + 1 << endl;
		cin >> expo;

		head = insert(head, coeff, expo);
	}
	return head;
}

void print(struct Node* head) {
	if (head == nullptr) {
		cout << "No polynomial";
		return;
	}
	struct Node* temp = head;
	while (temp != nullptr) {
		cout << temp->coeff << "x^" << temp->expo;
		temp = temp->link;
		if (temp != nullptr) cout << " + ";
	}
	cout << endl;
}

struct Node* polyAdd(struct Node* head1, struct Node* head2) {
	struct Node* temp1 = head1;
	struct Node* temp2 = head2;
	struct Node* head3 = nullptr;
	while (temp1 != nullptr && temp2 != nullptr) {
		if (temp1->expo == temp2->expo) {
			head3 = insert(head3, temp1->coeff + temp2->coeff, temp1->expo);
			temp1 = temp1->link;
			temp2 = temp2->link;
		}
		else if (temp1->expo > temp2->expo) {
			head3 = insert(head3, temp1->coeff, temp1->expo);
			temp1 = temp1->link;
		}
		else if (temp2->expo > temp1->expo) {
			head3 = insert(head3, temp2->coeff, temp2->expo);
			temp2 = temp1->link;
		}
		
	}
	while (temp1 != nullptr) {
		head3 = insert(head3, temp1->coeff, temp1->expo);
		temp1 = temp1->link;
	}
	while (temp2 != nullptr) {
		head3 = insert(head3, temp2->coeff, temp2->expo);
		temp2 = temp2->link;
	}
	return head3;
}

struct Node* polyMultiplication(struct Node* head1, struct Node* head2) {
	struct Node* temp1 = head1;
	struct Node* temp2 = head2;
	struct Node* head3 = nullptr;

	while (temp1 != nullptr) {
		while (temp2 != nullptr) {
			head3 = insert(head3, temp1->coeff * temp2->coeff, temp1->expo + temp2->expo);
			temp2 = temp2->link;
		}
		temp1 = temp1->link;
		temp2 = head2;
	}
	print(head3);
	

	struct Node* temp3 = head3;
	struct Node* tempPtr = nullptr;

	while (temp3->link != nullptr) {
		if (temp3->expo == temp3->link->expo) {
			temp3->coeff = temp3->coeff + temp3->link->coeff;
			tempPtr = temp3->link;
			temp3->link = temp3->link->link;
			delete tempPtr;
		}
		else {
			temp3 = temp3->link;
		}
	}

	print(head3);


	return head3;
}

int main()
{
	struct Node* head1 = nullptr;
	struct Node* head2 = nullptr;
	cout << "Enter the first polynomial" << endl;
	head1 = create(head1);
	cout << "Enter the second polynomial" << endl;
	head2 = create(head2);

	//polyAdd(head1, head2);
	polyMultiplication(head1, head2);

	return 0;
}

