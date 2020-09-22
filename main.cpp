/* Cache oblivious PVEB trees */


#include <bits/stdc++.h> 
using namespace std; 

class pveb_tree { 

public: 
	
	int u; 

	pveb_tree* summary; 

	vector<pveb_tree*> clusters; 

	int root(int u) 
	{ 
		return floor(sqrt(u)); 
	} 

	int high(int x) 
	{ 
		return x/root(u); 
	} 

	int low(int x) 
	{ 
		return x%root(u); 
	} 

	int get_index(int cluster, int pos) 
	{ 
		return cluster*root(u)+pos; 
	} 

	
	pveb_tree(int size) 
	{ 
		u = size; 

		if (size <= 2) { 

			summary = NULL; 

			clusters = vector<pveb_tree*>(size, NULL); 
		} 
		else { 

			summary = new pveb_tree(root(size)); 

			clusters = vector<pveb_tree*>(root(size), NULL); 

			for (int i = 0; i < root(size); i++) { 
				clusters[i] = new pveb_tree(root(size)); 
			} 
		} 
	} 
}; 

bool present_page(pveb_tree* helper, int key) 
{ 

	if (key >= helper->u) 
		return false; 

	if (helper->u == 2) { 
		return helper->clusters[key]; 
	} 
	else { 

		return present_page(helper->clusters[helper->high(key)], 
						helper->low(key)); 
	} 
} 

void insert(pveb_tree*& helper, int key) 
{ 
	if (helper->u == 2) { 
		helper->clusters[key] = new pveb_tree(1); 
	} 
	else { 

		insert(helper->clusters[helper->high(key)], 
			helper->low(key)); 

		insert(helper->summary, helper->high(key)); 
	} 
} 

int minimum(pveb_tree* helper) 
{ 
	if (helper->u == 2) { 
		if (helper->clusters[0]) { 
			return 0; 
		} 
		else if (helper->clusters[1]) { 
			return 1; 
		} 

		return -1; 
	} 
	else { 

		int min_cluster = minimum(helper->summary); 
		int offset; 

		if (min_cluster == -1) { 
			return -1; 
		} 
		else { 

			offset = minimum(helper->clusters[min_cluster]); 

			return helper->get_index(min_cluster, offset); 
		} 
	} 
} 

int maximum(pveb_tree* helper) 
{ 

	if (helper->u == 2) { 
		if (helper->clusters[1]) { 
			return 1; 
		} 
		else if (helper->clusters[0]) { 
			return 0; 
		} 

		return -1; 
	} 
	else { 

		int max_cluster = maximum(helper->summary); 
		int offset; 

		if (max_cluster == -1) { 
			return -1; 
		} 
		else { 

			offset = maximum(helper->clusters[max_cluster]); 
			return helper->get_index(max_cluster, offset); 
		} 
	} 
} 

int successor(pveb_tree* helper, int key) 
{ 
	if (helper->u == 2) { 
		if (key == 0 && helper->clusters[1]) 
			return 1; 
		else
			return -1; 
	} 
	else { 

		int offset = successor(helper->clusters[helper->high(key)], 
							helper->low(key)); 

		if (offset != -1) 
			return helper->get_index(helper->high(key), offset); 
		else { 

			int successor_cluster = successor(helper->summary, 
											helper->high(key)); 

			if (successor_cluster == -1) 
				return -1; 
			else { 

				offset = minimum(helper->clusters[successor_cluster]); 

				return helper->get_index(successor_cluster, offset); 
			} 
		} 
	} 
} 

int predecessor(pveb_tree* helper, int key) 
{ 

	if (helper->u == 2) { 
		if (key == 1 && helper->clusters[0]) 
			return 0; 
		else
			return -1; 
	} 
	else { 

		int offset = predecessor(helper->clusters[helper->high(key)], 
								helper->low(key)); 

		if (offset != -1) 
			return helper->get_index(helper->high(key), offset); 
		else { 

			int predecessor_cluster = predecessor(helper->summary, 
												helper->high(key)); 

			if (predecessor_cluster == -1) 
				return -1; 
			else { 

				offset = maximum(helper->clusters[predecessor_cluster]); 

				return helper->get_index(predecessor_cluster, offset); 
			} 
		} 
	} 
} 

void pveb_delete(pveb_tree*& helper, int key) 
{ 

	if (helper->u == 2) { 
		if (helper->clusters[key]) { 
			delete helper->clusters[key]; 
			helper->clusters[key] = NULL; 
		} 
	} 
	else { 

		pveb_delete(helper->clusters[helper->high(key)], helper->low(key)); 

		bool non_empty_cluster = false; 

		for (int i = helper->high(key) * helper->root(helper->u); 
			i < (helper->high(key) + 1) * helper->root(helper->u); 
			i++) { 

			if (present_page(helper->clusters[helper->high(key)], i)) { 
				non_empty_cluster = true; 
				break; 
			} 
		} 

		if (non_empty_cluster == false) { 
			pveb_delete(helper->summary, helper->high(key)); 
		} 
	} 
} 

int main(int argc, char const *argv[])
{
	int frames;
	cout<<"enter number of frames :- ";
	cin>>frames; 
	
	pveb_tree* pagetable = new pveb_tree(frames); 

	int loop = 1;
	int choice;
	int ele;
	while(loop)
	{
		cout<<"\n\n1) Insert a page \n"<<"2) Delete a page \n"<<"3) Check if a page is present \n"<<"4) Get the first allocated page number \n"<<"5) Get the last allocated page number\n"<<"6) Get next page number(successor) \n"<<"7) Get previous page number(predecessor)\n"<<"0) Exit\n";

		cin>>choice;

		switch(choice)
		{
			case 1:
					cout<<"Enter frame number :- ";
					cin>>ele;
					insert(pagetable,ele);
					break;

			case 2:
				cout<<"Enter page number to delete :- ";
				cin>>ele;
				pveb_delete(pagetable,ele);
				break;


			case 3:
				cout<<"Enter page number to check :- ";
				cin>>ele;
				if(present_page(pagetable,ele))
				{
					cout<<"Present\n";
				}
				else cout<<"Not present\n";
				break;

			case 4:
				cout<<"page number :- "<<minimum(pagetable)<<endl;
				break;

			case 5:
				cout<<"page number :- "<<maximum(pagetable)<<endl;
				break;

			case 6:
				cout<<"Enter a page number :- ";
				cin>>ele;
				cout<<"next page number :- "<<successor(pagetable,ele)<<endl;
				break;
			case 7:
				cout<<"Enter a page number :- ";
				cin>>ele;
				cout<<"previous page number :- "<<predecessor(pagetable,ele)<<endl;
				break;

			default:
				loop = 0;
				break;

		}

	}

	return 0;

} 
