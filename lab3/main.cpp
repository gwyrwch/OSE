#include <iostream>

using namespace std;

struct ListNode {
  string* val;
  ListNode *next;
  ListNode *prev;
  ListNode(string* x) : val(x), next(nullptr), prev(nullptr) {}
};
 
ListNode* insert_begin(ListNode* l, string s) {
	if (l == nullptr) {
		l = new ListNode(new string(s));
		return l;
	}

	l->prev = new ListNode(new string(s));
	l->prev->next = l;
	l = l->prev;
	return l;
}

ListNode* insert_end(ListNode* l, string s) {
	if (l == nullptr) {
		l = new ListNode(new string(s));
		return l;
	}

	ListNode* t = l;
	while(t->next != nullptr) {
		t = t->next;
	}

	t->next = new ListNode(new string(s));
	t->next->prev = t;

	return l;
}

ListNode* delete_node(ListNode* l, int index) {
	ListNode* t = l;

	if (l == nullptr)
		return l;

	if (index == 0) {
		if (t->next) {
			t = t->next;
			t->prev = nullptr;
			delete l;

			return t;
		}
		delete l;
		return nullptr;
	}

	int cnt = 0;
	while (cnt != index - 1 && t != nullptr) {
		t = t->next;
		cnt++;
	}

	if (t == nullptr || t->next == nullptr)
		return l;

	ListNode* t_next = t->next;
	t->next = t->next->next;
	if (t->next != nullptr) {
		t->next->prev = t;
	}

	delete t_next;

	return l;
}

ListNode* clear(ListNode* l) {
	while (l != nullptr) {
		delete_node(l, 0);
	}

	return nullptr;
}

void print(ListNode* l) {
	if (l == nullptr) {
		cout << "{}\n";
		return;
	}

	cout << "{";
    while(l != NULL) {
    	if (l->next != nullptr)
        	cout << *l->val << ", ";
        else
        	cout << *l->val;
        l = l->next;
    }

    cout << "}\n";
}

int main() {
	ListNode* head = new ListNode(new string("3"));
	head = insert_begin(head, "2");
	head = insert_begin(head, "1");
	head = insert_end(head, "4");
	head = insert_end(head, "5");

	print(head);

	head = delete_node(head, 0);
	print(head);

	head = delete_node(head, 3);
	print(head);

	head = delete_node(head, 1);
	print(head);

	head = delete_node(head, 1);
	print(head);

	head = delete_node(head, 0);
	print(head);

	head = insert_end(head, "1");
	print(head);

	return 0;
}
