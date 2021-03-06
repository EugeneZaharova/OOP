#include "ntree.h"
#include <exception>
using namespace std;

template <class T> TNTree<T>::TNTree()
{
	this->root = nullptr;
}
template <class T> TNTree<T>::TNTree(shared_ptr<TNTree<T>> &orig)
{
	this->root = orig->root;
}
template <class T> shared_ptr<TNode<T>> TNTree<T>::Search(shared_ptr<T> sh)
{
	if (root->GetShape() == sh) {
		return root;
	}
	else {
		return root->search_rec(sh);
	}
}
template <class T> shared_ptr<TNode<T>> TNTree<T>::Search_Path(char *path)
{
	shared_ptr<TNode<T>> NodePath = nullptr;
	shared_ptr<TNode<T>> prev = nullptr;
	while (*path != '\0') {
		if (*path == 'r') {
			NodePath = this->root;
			prev = nullptr;
			path++;
			continue;
		}
		if (*path == 's') {
			prev = NodePath;
			if (NodePath->Son()) {
				NodePath = NodePath->Son();
			}
			path++;
			continue;
		}
		if (*path == 'b') {
			prev = NodePath;
			if (NodePath->Brother()) {
				NodePath = NodePath->Brother();
			}
			path++;
			continue;
		}
	}
	return NodePath;
}
template <class T> void TNTree<T>::Insert(shared_ptr<T> sh, char *path, char *who)
{
	shared_ptr<TNode<T>> nd (new TNode<T>(sh));
	if ((this->root == nullptr)) {
		this->root = nd;
	}
	else {
		shared_ptr<TNode<T>> Nodepath = Search_Path(path);
		if (*who == 's') {
				Nodepath->SetSon(nd);
				nd->SetParent(Nodepath);
		}
		if (*who == 'b') {
			if (Nodepath == root) {
				cout << "ROOT CAN'T HAVE BROTHERS" << endl;
			}
			else {
				Nodepath->SetBrother(nd);
				nd->SetParent(Nodepath->Parent());
			}
		}
	}
}
template <class T> void TNTree<T>::Delete(char *path)
{
	shared_ptr<TNode<T>> node = this->Search_Path(path);
	if (node == this->root) {
		this->root = nullptr;
		return;
	}
	if (!(node->Brother()) && !(node->Son()) && node == node->Parent()->Son()) {
		node = node->Parent();
		node->SetSon(nullptr);
	}
	shared_ptr<TNode<T>> tmp = node;
	shared_ptr<TNode<T>> tmp_older_brother = nullptr;
	shared_ptr<TNode<T>> tmp_parent = nullptr;
	while (tmp->Son()) {
		tmp_parent = tmp;
		tmp = tmp->Son();
	}
	while (tmp->Brother()) {
		tmp_older_brother = tmp;
		tmp = tmp->Brother();
	}
	if (tmp != node) {
		node->SetShape(tmp->GetShape());
		tmp_older_brother->SetBrother(nullptr);
	}
	tmp = nullptr;
	
}
template <class T> void TNTree<T>::Print(char *path)
{
	shared_ptr<TNode<T>> Path = this->Search_Path(path);
	if (Path == nullptr) {
		cout << "DIRECTORY IS NULLPTR" << endl;
		return;
	}
	shared_ptr<TShape> shape = Path->GetShape();
	cout << "Tree:";
	while (*path) {
		cout << "/" << *path;
		path++;
	}
	cout << "  ";
	shape->Print();
}
template <class T> shared_ptr<TNode<T>> TNTree<T>::Minimum() //minimum is the deepest son
{
	shared_ptr<TNode<T>> node = this->root;
	while (node->Son()) {
		node = node->Son();
	}
	return node;
}
template <class T> shared_ptr<TNode<T>> TNTree<T>::Maximum() //maximum is the root
{
	return this->root;
}
template <class T> TIterator<TNode<T>, T> TNTree<T>::begin() //begin == minimum
{
	return TIterator<TNode<T>, T>(this->Minimum());
}
template <class T> TIterator<TNode<T>, T> TNTree<T>::end() // end is the maximum is the root
{
	return TIterator<TNode<T>, T>(this->Maximum());
}
template <class T> size_t TNTree<T>::Size()
{
	if (this->root == nullptr) {
		return 0;
	}
	size_t result = 0;
	TIterator<TNode<T>, T> it_loc = this->begin();
	TIterator<TNode<T>, T> end_loc = this->end();
	for (it_loc, end_loc; it_loc != end_loc; it_loc++) {
		result++;
	}
	result++;
	return result;
}
template <class T> future<void> TNTree<T>::sort_bg()
{
	packaged_task<void(void)> task(bind(mem_fn(&TNTree<T>::parallel_sort), this));
	future<void> res(task.get_future());
	thread th(move(task));
	th.detach();
	return res;
}
template <class T> shared_ptr<T> TNTree<T>::operator [] (size_t i)
{
	if (i < this->Size() - 1) {
		throw
			invalid_argument("index greater then tree size");
	}
	size_t j = 0;
	for (shared_ptr<T> a : *this) {
		if (j == i) {
			return a;
		}
		j++;
	}
	return shared_ptr<T>(nullptr);
}
template <class T> void TNTree<T>::sort()
{
	if (this->Size() > 1) {

	}
}
template <class T> void TNTree<T>::parallel_sort()
{

}
template <class T> TNTree<T>::~TNTree()
{
	while (this->root != nullptr) {
		this->Delete("r");
	}
	this->root = nullptr;
}

#include "Shape.h"
template class TNTree<TShape>;