#include "main.h"
int MAXSIZE;

class cusNode // Lưu lịch sử khách hàng
{
public:
	int result;
	int ID;

public:
	cusNode()
	{
		this->result = 0;
		this->ID = 0;
	}
	cusNode(int result, int ID)
	{
		this->result = result;
		this->ID = ID;
	}
};
class sukanaNode // Node in minheap of sukana Restaurant
{
private:
	int numOfCustomer;
	int areaLabel;
	vector<cusNode *> listCusInArea;
	friend class Restaurant;
	friend class SukanaRestaurant;

public:
	sukanaNode()
	{
		this->numOfCustomer = 0;
		this->areaLabel = 0;
	}
	sukanaNode(int numGuest, int label)
	{
		this->numOfCustomer = numGuest;
		this->areaLabel = label;
	}
};
class SukanaRestaurant // Restaurant of Sukana
{
private:
	vector<sukanaNode *> arrayOfAreas;
	vector<sukanaNode *> minHeap;
	vector<int> checkInList;
	friend class Restaurant;

public:
	SukanaRestaurant()
	{
		this->arrayOfAreas.resize(MAXSIZE);
		for (int i = 0; i < MAXSIZE; i++)
		{
			arrayOfAreas[i] = new sukanaNode(0, i + 1);
		}
		this->minHeap.resize(0);
		this->checkInList.resize(0);
	}
	~SukanaRestaurant()
	{
		for (int i = 0; i < MAXSIZE; i++)
		{
			delete arrayOfAreas[i];
		}
		for (int i = 0; i < minHeap.size(); i++)
		{
			delete minHeap[i];
		}
	}
	int totalCustomerInRestaurant()
	{
		int total = 0;
		for (int i = 0; i < this->minHeap.size(); i++)
		{
			total += this->minHeap[i]->numOfCustomer;
		}
		return total;
	}
	void printPreOderHeap(int position, int number)
	{
		if (position >= this->minHeap.size() || number <= 0)
		{
			return;
		}
		int numAtNode = min(number, this->minHeap[position]->numOfCustomer);
		for (int i = 0; i < numAtNode; i++)
		{
			cout << this->minHeap[position]->listCusInArea[i]->ID << "-" << this->minHeap[position]->listCusInArea[i]->result << endl;
		}
		number -= numAtNode;
		if (number > 0)
		{
			printPreOderHeap(2 * position + 1, number);
			int numThatPrinted = this->totalCustomerInRestaurant() - number;
			int remainNumToPrint = this->totalCustomerInRestaurant() - numThatPrinted;
			printPreOderHeap(2 * position + 2, remainNumToPrint);
		}
	}

	void getOutCus(int area, int numOfGuest)
	{
		if (numOfGuest > this->arrayOfAreas[area - 1]->numOfCustomer)
		{
			numOfGuest = this->arrayOfAreas[area - 1]->numOfCustomer;
		}
		for (int i = 0; i < numOfGuest; i++)
		{
			for (int i = 0; i < this->checkInList.size(); i++) // Xoa checkin List
			{
				if (this->checkInList[i] == area)
				{
					this->checkInList.erase(this->checkInList.begin() + i);
					break;
				}
			}
			this->arrayOfAreas[area - 1]->numOfCustomer--; // Xoa mang tinh
			for (int i = 0; i < this->minHeap.size(); i++)
			{
				if (this->minHeap[i]->areaLabel == area)
				{
					this->minHeap[i]->numOfCustomer--;
					if (this->minHeap[i]->numOfCustomer == 0)
					{
						this->deleteMinHeap(this->minHeap, area);
					}
					else
					{
						this->reheapUp(i, false);
					}
					break;
				}
			}
		}
	}

	void mergeSukanaNode(vector<sukanaNode *> &nodeList, int start, int step)
	{
		for (int i = step + start; i < nodeList.size(); i += step)
		{
			int j = i;
			while (j > start && nodeList[j]->numOfCustomer <= nodeList[j - step]->numOfCustomer)
			{
				if (nodeList[j]->numOfCustomer < nodeList[j - step]->numOfCustomer)
				{
					swap(nodeList[j], nodeList[j - step]);
					j = j - step;
				}
				else
				{
					if (this->x1longerServicex2(nodeList[j]->areaLabel, nodeList[j - step]->areaLabel))
					{
						swap(nodeList[j], nodeList[j - step]);
						j = j - step;
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	void mergeSortSukanaNode(vector<sukanaNode *> &nodeList)
	{
		int k = nodeList.size();
		while (k > 0)
		{
			for (int i = 0; i < k; i++)
			{
				this->mergeSukanaNode(nodeList, i, k);
			}
			k /= 2;
		}
	}

	vector<sukanaNode *> minCusArea(int number)
	{
		vector<sukanaNode *> result;
		vector<sukanaNode *> temp = this->minHeap;
		this->mergeSortSukanaNode(temp);
		if (number > temp.size())
		{
			number = temp.size();
		}
		for (int i = 0; i < number; i++)
		{
			result.push_back(temp[i]);
		}
		return result;
	}
	void reheapUp(int position, bool in)
	{
		if (position % 2 == 0)
		{
			while ((position - 2) / 2 >= 0 && this->minHeap[position]->numOfCustomer <= this->minHeap[(position - 2) / 2]->numOfCustomer)
			{
				if (this->minHeap[position]->numOfCustomer <= this->minHeap[(position - 2) / 2]->numOfCustomer)
				{
					if (!in)
					{
						swap(this->minHeap[position]->numOfCustomer, this->minHeap[(position - 2) / 2]->numOfCustomer);
						position = (position - 2) / 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					swap(this->minHeap[position]->numOfCustomer, this->minHeap[(position - 2) / 2]->numOfCustomer);
					position = (position - 2) / 2;
				}
			}
		}
		else
		{
			while ((position - 1) / 2 >= 0 && this->minHeap[position]->numOfCustomer <= this->minHeap[(position - 1) / 2]->numOfCustomer)
			{
				if (this->minHeap[position]->numOfCustomer <= this->minHeap[(position - 1) / 2]->numOfCustomer)
				{
					if (!in)
					{
						swap(this->minHeap[position]->numOfCustomer, this->minHeap[(position - 1) / 2]->numOfCustomer);
						position = (position - 1) / 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					swap(this->minHeap[position]->numOfCustomer, this->minHeap[(position - 1) / 2]->numOfCustomer);
					position = (position - 1) / 2;
				}
			}
		}
	}
	void reheapDown(int position, bool in)
	{
		int l = 2 * position + 1;
		int r = 2 * position + 2;
		if (this->minHeap[l]->numOfCustomer < this->minHeap[r]->numOfCustomer)
		{
			while (l < MAXSIZE && this->minHeap[position]->numOfCustomer >= this->minHeap[l]->numOfCustomer)
			{
				if (this->minHeap[position]->numOfCustomer == this->minHeap[l]->numOfCustomer)
				{
					if (in)
					{
						swap(this->minHeap[position], this->minHeap[l]);
						reheapDown(l, in);
					}
					else
					{
						break;
					}
				}
				else
				{

					swap(this->minHeap[position], this->minHeap[l]);
					reheapDown(l, in);
				}
			}
		}
		else
		{
			while (r < MAXSIZE && this->minHeap[position]->numOfCustomer >= this->minHeap[r]->numOfCustomer)
			{
				if (this->minHeap[position]->numOfCustomer == this->minHeap[r]->numOfCustomer)
				{
					if (in)
					{
						swap(this->minHeap[position], this->minHeap[r]);
						reheapDown(r, in);
					}
					else
					{
						break;
					}
				}
				else
				{
					swap(this->minHeap[position], this->minHeap[r]);
					reheapDown(r, in);
				}
			}
		}
	}

	bool containAreaInMinHeap(int areaNumber)
	{
		for (int i = 0; i < this->minHeap.size(); i++)
		{
			if (this->minHeap[i]->areaLabel == areaNumber)
			{
				return true;
			}
		}
		return false;
	}

	void heapify(vector<sukanaNode *> &minheap, sukanaNode *newArea)
	{
		minheap.push_back(newArea);
		this->reheapUp(minheap.size() - 1, true);
		this->reheapDown(minheap.size() - 1, true);
	}

	bool x1longerServicex2(int area1, int area2)
	{
		bool Area1 = false, Area2 = false;
		for (int i = 0; i < this->checkInList.size(); i++)
		{
			if (this->checkInList[i] == area1)
			{
				Area1 = true;
			}
			if (this->checkInList[i] == area2)
			{
				Area2 = true;
			}
			if (Area1 == false && Area2 == true)
			{
				return false;
			}
		}
		return true;
	}

	void deleteMinHeap(vector<sukanaNode *> &Heap, int area)
	{
		int index = 0;
		for (int i = 0; i < Heap.size(); i++)
		{
			if (Heap[i]->areaLabel == area)
			{
				index = i;
			}
		}
		Heap[index] = Heap[Heap.size() - 1];
		Heap.pop_back();
		this->reheapDown(index, false);
	}

	void welcomeToSukanaRestaurant(int id)
	{
		this->checkInList.push_back(id);
		this->arrayOfAreas[id - 1]->numOfCustomer++;
		if (!this->containAreaInMinHeap(id))
		{
			sukanaNode *temp = this->arrayOfAreas[id - 1];
			this->heapify(this->minHeap, temp);
			this->reheapUp(id - 1, true);
			this->reheapDown(id - 1, true);
		}
		else
		{
			for (int i = 0; i < this->minHeap.size(); i++)
			{
				if (this->minHeap[i]->areaLabel == id)
				{
					this->minHeap[i]->numOfCustomer++;
					this->reheapDown(i, true);
					break;
				}
			}
		}
	}
};

class GojoRestaurant
{
public:
	class Node;

private:
	vector<Node *> arrayOfRoot; //Danh sach cac root cua cac khu vuc trong nha hang
	vector<vector<Node *>> historyCustomer; //Danh sach khach hang cua tung khu vuc
	friend class Restaurant;

public:
	GojoRestaurant()
	{
		this->arrayOfRoot.resize(MAXSIZE);
		for (int i = 0; i < MAXSIZE; i++)
		{
			arrayOfRoot[i] = nullptr;
		}
		this->historyCustomer.resize(MAXSIZE);
	}
	int countNode(Node *r)
	{
		if (!r)
		{
			return 0;
		}
		return 1 + countNode(r->left) + countNode(r->right);
	}
	long long Combination(int n, int k)
	{
		vector<vector<long long>> combination; // pre calculate for Combination
		combination.resize(n + 1);
		for (int i = 0; i <= n; i++)
		{
			combination[i].resize(n + 1, 0);
		}
		for (int i = 1; i <= n; i++)
		{
			combination[0][i] = 1;
		}

		for (int j = 1; j <= n; j++)
		{
			for (int i = 1; i <= j; i++)
			{
				if (i == j)
				{
					combination[i][j] = 1;
				}
				else if (i == 1)
				{
					combination[i][j] = j;
				}
				else
				{
					combination[i][j] = combination[i - 1][j - 1] + combination[i][j - 1];
				}
			}
		}
		return combination[k][n];
	}
	int numberOfOrderArray(Node *r)
	{
		int numberOfElement = this->countNode(r);
		if (numberOfElement <= 2)
		{
			return 1;
		}
		return (this->Combination(numberOfElement - 1, this->countNode(r->left)) * numberOfOrderArray(r->left) * numberOfOrderArray(r->right)) % MAXSIZE;
	}

	void BSTToArray(Node *root, vector<int> &result)
	{
		if (!root)
		{
			return;
		}
		BSTToArray(root->left, result);
		BSTToArray(root->right, result);
		result.push_back(root->number);
	}
	void printInOrderBST(Node *root)
	{
		if (!root)
		{
			return;
		}
		printInOrderBST(root->left);
		cout << root->number << endl;
		printInOrderBST(root->right);
	}
	Node *insertBST(Node *newNode, Node *root)
	{
		if (!root)
		{
			root = newNode;
			return root;
		}
		if (newNode->number < root->number)
		{
			root->left = insertBST(newNode, root->left);
		}
		else
		{
			root->right = insertBST(newNode, root->right);
		}
		return root;
	}
	void welcomeToGojo(int res, int id)
	{
		Node *temp = new Node(res);
		if (!this->arrayOfRoot[id - 1])
		{
			this->arrayOfRoot[id - 1] = temp;
		}
		else
		{
			this->arrayOfRoot[id - 1] = this->insertBST(temp, this->arrayOfRoot[id - 1]);
		}
		this->historyCustomer[id - 1].push_back(temp);
	}
	Node *searchBST(Node *root, int key)
	{
		if (!root || root->number == key)
		{
			return root;
		}
		if (key < root->number)
		{
			return searchBST(root->left, key);
		}
		else
		{
			return searchBST(root->right, key);
		}
	}
	Node *minNode(Node *root)
	{
		if (!root)
		{
			return root;
		}
		Node *temp = root;
		while (temp->left)
		{
			temp = temp->left;
		}
		return temp;
	}
	Node *deleteBST(int number, Node *root)
	{
		Node *temp = this->searchBST(root, number);
		if (temp)
		{
			if (number < root->number)
			{
				root->left = deleteBST(number, root->left);
			}
			else if (number > root->number)
			{
				root->right = deleteBST(number, root->right);
			}
			else
			{
				if (!root->left)
				{
					Node *temp = root->right;
					delete root;
					return temp;
				}
				else if (!root->right)
				{
					Node *temp = root->left;
					delete root;
					return temp;
				}
				else
				{
					Node *temp = this->minNode(root->right);
					root->number = temp->number;
					this->deleteBST(temp->number, root->right);
				}
			}
		}
		return root;
	}
	void getCustomerOut(int num, int area)
	{
		if (num > this->countNode(this->arrayOfRoot[area - 1]))
		{
			num = this->countNode(this->arrayOfRoot[area - 1]);
		}
		for (int i = 0; i < num; i++)
		{
			this->arrayOfRoot[area - 1] = this->deleteBST(this->historyCustomer[area - 1][0]->number, this->arrayOfRoot[area - 1]);
			this->historyCustomer[area - 1].erase(historyCustomer[area - 1].begin());
		}
	}
	~GojoRestaurant()
	{
		for (int i = 0; i < MAXSIZE; i++)
		{
			delete arrayOfRoot[i];
		}
	}
	class Node
	{
	private:
		int number;
		Node *left;
		Node *right;

		friend class GojoRestaurant;

	public:
		Node()
		{
			this->number = 0;
			this->left = nullptr;
			this->right = nullptr;
		}
		Node(int num)
		{
			this->number = num;
			this->left = nullptr;
			this->right = nullptr;
		}
	};
};
class HuffmanNode
{
private:
	int frequency;
	char charName;
	HuffmanNode *left;
	HuffmanNode *right;

	friend class Restaurant;
	friend class HuffmanTree;

public:
	bool isLeafNode()
	{
		if (this->left == nullptr && this->right == nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	HuffmanNode()
	{
		this->frequency = 0;
		this->charName = ' ';
		this->left = nullptr;
		this->right = nullptr;
	}
	HuffmanNode(int freq, char charName)
	{
		this->frequency = freq;
		this->charName = charName;
		this->left = nullptr;
		this->right = nullptr;
	}
};

class HuffmanTree // as AVL tree
{
private:
	HuffmanNode *root;
	friend class Restaurant;

public:
	HuffmanTree()
	{
		this->root = nullptr;
	}
	bool balanceTotal(HuffmanNode *root)
	{
		if (!root)
		{
			return true;
		}
		if (abs(this->getBalanceFactor(root)) > 1)
		{
			return false;
		}
		return balanceTotal(root->left) && balanceTotal(root->right);
	}
	void printTree(HuffmanNode *root, int space, std::vector<std::string> &lines)
	{
		if (root == nullptr)
		{
			return;
		}

		// Khoảng cách giữa các nút
		const int spacing = 3;
		space += spacing;

		// Xử lý nút bên phải trước
		printTree(root->left, space, lines);

		// Chuẩn bị dòng để in nút hiện tại
		std::string line(space, ' ');
		line += std::to_string(root->frequency);
		if (isalpha(root->charName))
		{
			line += root->charName;
		}
		lines.push_back(line);

		// Xử lý nút bên trái
		printTree(root->right, space, lines);
	}

	void printTreeWrapper(HuffmanNode *root)
	{
		std::vector<std::string> lines;
		printTree(root, 0, lines);

		// In ngược các dòng để có đúng trật tự
		std::reverse(lines.begin(), lines.end());
		for (auto &line : lines)
		{
			std::cout << line << std::endl;
		}
	}
	void printInOrder(HuffmanNode *root)
	{
		if (!root)
		{
			return;
		}
		printInOrder(root->left);
		if (isalpha(root->charName))
		{
			cout << root->charName << endl;
		}
		else
		{
			cout << root->frequency << endl;
		}
		printInOrder(root->right);
	}
	long long binaryToDecimal(string binary)
	{
		int j = 0;
		long long res = 0;
		int count = binary.length();
		if (count > 10)
		{
			count = 10;
		}
		string newBinary = "";
		for (int i = binary.length() - 1; count > 0; i--)
		{
			newBinary += binary[i];
			count--;
		}
		for (int i = newBinary.length() - 1; i >= 0; i--)
		{
			res += (newBinary[i] - '0') * pow(2, j);
			j++;
		}

		return res;
	}
	void generateHuffmanCodes(HuffmanNode *root, const string &code, unordered_map<char, string> &huffmanCodes)
	{
		if (root == nullptr)
			return;

		// Found a leaf node
		if (!root->left && !root->right)
		{
			huffmanCodes[root->charName] = code;
		}
		generateHuffmanCodes(root->left, code + "0", huffmanCodes);
		generateHuffmanCodes(root->right, code + "1", huffmanCodes);
	}
	int getHeight(HuffmanNode *r)
	{
		if (!r)
		{
			return -1;
		}
		return max(getHeight(r->left), getHeight(r->right)) + 1;
	}
	int getBalanceFactor(HuffmanNode *r)
	{
		return getHeight(r->left) - getHeight(r->right);
	}
	HuffmanNode *rotateRight(HuffmanNode *r, bool &rotated)
	{
		if (!r)
		{
			return r;
		}
		rotated = true;
		HuffmanNode *temp1 = r->left;
		HuffmanNode *temp2 = temp1->right;
		temp1->right = r;
		r->left = temp2;
		return temp1;
	}
	HuffmanNode *rotateLeft(HuffmanNode *r, bool &rotated)
	{
		if (!r)
		{
			return r;
		}
		rotated = true;

		HuffmanNode *temp1 = r->right;
		HuffmanNode *temp2 = temp1->left;
		temp1->left = r;
		r->right = temp2;
		return temp1;
	}
	HuffmanNode *insertToHuffmanTree(HuffmanNode *Node1, HuffmanNode *Node2, HuffmanNode *r)
	{
		if (!r)
		{
			r = Node1;
			return r;
		}
		r->left = Node1;
		r->right = Node2;
		return r;
	}

	HuffmanNode *rotate(HuffmanNode *r, bool &rotated)
	{
		if (!r || rotated)
		{
			return r;
		}

		int bf = getBalanceFactor(r);
		if (bf > 1 && getBalanceFactor(r->left) >= 0)
		{
			return rotateRight(r, rotated);
		}
		else if (bf < -1 && getBalanceFactor(r->right) <= 0)
		{
			return rotateLeft(r, rotated);
		}
		else if (bf > 1 && getBalanceFactor(r->left) < 0)
		{
			r->left = rotateLeft(r->left, rotated);
			return rotateRight(r, rotated);
		}
		else if (bf < -1 && getBalanceFactor(r->right) > 0)
		{
			r->right = rotateRight(r->right, rotated);
			return rotateLeft(r, rotated);
		}

		if (!rotated)
		{
			r->left = rotate(r->left, rotated);
		}
		if (!rotated)
		{
			r->right = rotate(r->right, rotated);
		}
		return r;
	}
};

class Restaurant
{
private:
	HuffmanTree *HuffTree;
	vector<HuffmanNode *> HuffmanNodeQueue;
	GojoRestaurant *gojoRestaurant;
	SukanaRestaurant *sukanaRestaurant;
	vector<HuffmanNode *> HuffmanNodeListToHand;

public:
	Restaurant()
	{
		this->HuffTree = new HuffmanTree();
		this->gojoRestaurant = new GojoRestaurant();
		this->sukanaRestaurant = new SukanaRestaurant();
	}
	~Restaurant()
	{
		delete this->HuffTree;
		delete this->gojoRestaurant;
		delete this->sukanaRestaurant;
	}
	int findFreqOfChar(vector<char> &charVec, vector<int> &freqVec, char c)
	{
		for (int i = 0; i < charVec.size(); i++)
		{
			if (charVec[i] == c)
			{
				return freqVec[i];
			}
		}
		return -1;
	}
	void frequencyOfChar(string str, vector<char> &charList, vector<int> &freqList)
	{
		for (int i = 0; i < str.length(); i++)
		{
			if (find(uniqueNameInput.begin(), uniqueNameInput.end(), str[i]) != uniqueNameInput.end())
			{
				// Loc ky tu bi lap
			}
			else
			{
				uniqueNameInput.push_back(str[i]);
			}
		}
		// for (int i = 0; i < uniqueNameInput.size(); i++)
		// {
		// 	cout << uniqueNameInput[i] << " ";
		// }
		int freq[52];
		fill(freq, freq + 52, 0);
		for (int i = 0; i < str.length(); i++)
		{
			if (str[i] >= 'a' && str[i] <= 'z')
			{
				freq[(str[i] - 'a') * 2]++;
			}
			else
			{
				freq[max(1, 2 * (str[i] - 'A') + 1)]++;
			}
		}

		for (int i = 0; i < 52; i++)
		{
			if (freq[i])
			{
				if (i % 2 == 0)
				{
					charList.push_back(char(i / 2 + 'a'));
					freqList.push_back(freq[i]);
				}
				else
				{
					charList.push_back(char(max(1, (i - 1) / 2 + 'A')));
					freqList.push_back(freq[i]);
				}
			}
		}
		unordered_map<char, int> mapCharAndFreq;
		for (int i = 0; i < charList.size(); i++)
		{
			mapCharAndFreq[charList[i]] = freqList[i];
		}
	}
	void mergeHuffNode(vector<HuffmanNode *> ls, int start = 0, int step = 1)
	{
		for (int i = start + step; i < ls.size(); i++)
		{
			int j = i;
			while (j > start && ls[j]->frequency < ls[j - step]->frequency)
			{
				swap(ls[j], ls[j - step]);
				j -= step;
			}
		}
	}
	void mergeSortHuffNode(vector<HuffmanNode *> ls)
	{
		int k = ls.size();
		while (k > 0)
		{
			for (int i = 0; i < k; i++)
			{
				mergeHuffNode(ls, i, k);
			}
			k /= 2;
		}
	}
	// Sort frequency of characters in name of customers
	void DoublyMerge(vector<char> &v1, vector<int> &v2, int start = 0, int step = 1)
	{
		for (int i = start + step; i < v2.size(); i += step)
		{
			int j = i;

			while (j > start && v2[j] <= v2[j - step])
			{
				if (v2[j] == v2[j - step])
				{
					if (!isupper(v1[j]) && isupper(v1[j - step]))
					{
						swap(v2[j], v2[j - step]);
						swap(v1[j], v1[j - step]);
						j -= step;
					}
					else
					{
						if (islower(v1[j]) && islower(v1[j - step]) && v1[j] - v1[j - step] < 0)
						{
							swap(v2[j], v2[j - step]);
							swap(v1[j], v1[j - step]);
							j -= step;
						}
						else if (isupper(v1[j]) && isupper(v1[j - step]) && v1[j] - v1[j - step] < 0)
						{
							swap(v2[j], v2[j - step]);
							swap(v1[j], v1[j - step]);
							j -= step;
						}
						else
						{
							break;
						}
					}
				}
				else
				{
					swap(v2[j], v2[j - step]);
					swap(v1[j], v1[j - step]);
					j -= step;
				}
			}
		}
	}
	// Sort frequency of characters in name of customers
	void DoubleMergeSort(vector<char> &v1, vector<int> &v2)
	{
		int k = v2.size() / 2;
		while (k > 0)
		{
			for (int i = 0; i < k; i++)
			{
				DoublyMerge(v1, v2, i, k);
			}
			k /= 2;
		}
	}
	vector<char> uniqueNameInput;
	void LAPSE(string name)
	{
		vector<char> initChar;
		vector<int> initFreq;
		this->frequencyOfChar(name, initChar, initFreq);

		// cout << "Truoc khi ma hoa Caesae" << endl;
		// for (int i = 0; i < initChar.size(); i++)
		// {
		// 	cout << initChar[i] << ": " << initFreq[i] << endl;
		// }
		// cout << "---------------------------------------------------------\n";
		if (initChar.size() >= 3)
		{
			string nameAfterCaesar = "";
			for (int i = 0; i < name.length(); i++)
			{
				char base;
				if (islower(name[i]))
				{
					base = 'a';
				}
				else
				{
					base = 'A';
				}
				nameAfterCaesar += static_cast<char>((name[i] - base + this->findFreqOfChar(initChar, initFreq, name[i])) % 26 + base);
			}
			// Ma hoa Caesar
			for (int i = 0; i < initChar.size(); i++)
			{
				char base;
				if (islower(initChar[i]))
				{
					base = 'a';
				}
				else
				{
					base = 'A';
				}
				initChar[i] = static_cast<char>((initChar[i] - base + initFreq[i]) % 26 + base);
			}

			// cout << "Sau khi ma hoa Caesae (chua cong don)" << endl;
			// for (int i = 0; i < initChar.size(); i++)
			// {
			// 	cout << initChar[i] << ": " << initFreq[i] << endl;
			// }
			// cout << "---------------------------------------------------------\n";

			// cong don sau khi ma hoa Ceasar

			vector<char> notSortChar;
			vector<int> notSortFreq;
			while (!initChar.empty())
			{
				notSortChar.push_back(initChar[0]);
				// cout << "push character: " << initChar[0] << " to new list\n";
				int temp = initFreq[0];
				char sampleChar = initChar[0];
				initChar.erase(initChar.begin());
				initFreq.erase(initFreq.begin());
				for (int i = 0; i < initChar.size(); i++)
				{
					if (initChar[i] == sampleChar)
					{
						temp += initFreq[i];
						initChar.erase(initChar.begin() + i);
						initFreq.erase(initFreq.begin() + i);
						i--;
					}
				}
				// cout << "total frequence  of " << notSortChar.back() << " is " << temp << endl;
				notSortFreq.push_back(temp);
			}
			// cout << "Sau khi ma hoa Caesae (da cong don)" << endl;
			// for (int i = 0; i < notSortChar.size(); i++)
			// {
			// 	cout << notSortChar[i] << ": " << notSortFreq[i] << endl;
			// }
			// cout << "---------------------------------------------------------\n";
			// for (int i = 0; i < initChar.size() - 1; i++)
			// {
			// 	for (int j = i + 1; j < initChar.size(); j++)
			// 	{
			// 		if (initChar[j] == initChar[i])
			// 		{
			// 			initFreq[i] += initFreq[j];
			// 			initChar.erase(initChar.begin() + j);
			// 			initFreq.erase(initFreq.begin() + j);
			// 		}
			// 	}
			// }

			DoubleMergeSort(notSortChar, notSortFreq);
			// cout << " Da sap xep" << endl;
			// for (int i = 0; i < initChar.size(); i++)
			// {
			// 	cout << initChar[i] << ": " << initFreq[i] << endl;
			// }
			// cout << "---------------------------------------------------------\n";
			// Tao vector cac node de buld Huffman tree
			for (int i = 0; i < notSortChar.size(); i++)
			{
				HuffmanNode *newNode = new HuffmanNode(notSortFreq[i], notSortChar[i]);
				this->HuffmanNodeQueue.push_back(newNode);
			}

			// Debug check vector cac node de buld Huffman tree
			// cout << "List node to build Huffman tree\n";
			// for (int i = 0; i < HuffmanNodeQueue.size(); i++)
			// {
			// 	cout << "char: " << HuffmanNodeQueue[i]->charName << " - " << HuffmanNodeQueue[i]->frequency << endl;
			// }

			while (HuffmanNodeQueue.size() > 1)
			{
				HuffmanNode *temp1 = HuffmanNodeQueue.front();
				// cout << "Left sub tree\n";
				// this->HuffTree->printTreeWrapper(temp1);
				// cout << "----------------\n";
				HuffmanNodeQueue.erase(HuffmanNodeQueue.begin());
				HuffmanNode *temp2 = HuffmanNodeQueue.front();
				// cout << "Right sub tree\n";
				// this->HuffTree->printTreeWrapper(temp2);
				// cout << "----------------\n";
				HuffmanNodeQueue.erase(HuffmanNodeQueue.begin());
				int tempRoot = temp1->frequency + temp2->frequency;
				HuffmanNode *temp3 = new HuffmanNode();
				temp3->frequency = tempRoot;
				this->HuffTree->root = this->HuffTree->insertToHuffmanTree(temp3, nullptr, this->HuffTree->root);
				this->HuffTree->root = this->HuffTree->insertToHuffmanTree(temp1, temp2, this->HuffTree->root);
				// Rotate (not fixed)
				int numRotate = 0;
				while (!this->HuffTree->balanceTotal(this->HuffTree->root) && numRotate < 3)
				{
					bool rotated = false;
					this->HuffTree->root = this->HuffTree->rotate(this->HuffTree->root, rotated);
					numRotate++;
					// cout << "Xoay lan thu: " << numRotate << endl;
				}
				// cout << "Sub tree after rotate\n";
				// this->HuffTree->printTreeWrapper(this->HuffTree->root);
				// cout << "---------------------------------------------\n";
				HuffmanNodeQueue.push_back(this->HuffTree->root);
				int bub = this->HuffmanNodeQueue.size() - 1;
				while (bub > 0 && HuffmanNodeQueue[bub]->frequency < HuffmanNodeQueue[bub - 1]->frequency)
				{
					swap(this->HuffmanNodeQueue[bub], this->HuffmanNodeQueue[bub - 1]);
					bub--;
				}
				if (this->HuffmanNodeQueue.size() > 1)
				{
					this->HuffTree->root = nullptr;
				}
			}
			this->HuffmanNodeListToHand.push_back(this->HuffTree->root);
			// ??????

			unordered_map<char, string> huffmanCodes;
			string res;
			this->HuffTree->generateHuffmanCodes(this->HuffTree->root, res, huffmanCodes);
			// string nameAfterCaesar="";

			string HuffmanBits = "";
			for (int i = 0; i < nameAfterCaesar.length(); i++)
			{
				HuffmanBits += huffmanCodes[nameAfterCaesar[i]];
			}
			// cout << "nameAfterCaesar: " << nameAfterCaesar << endl;
			// cout << "Bit string: ";
			// cout << HuffmanBits << endl;

			int result = this->HuffTree->binaryToDecimal(HuffmanBits);
			int ID = (result % MAXSIZE) + 1; // ID of customer (chay tu 1 cho den MAXSIZE)
			cout << "Result: " << result << endl;
			cout << "ID: " << ID << endl;
			// Su dung ket qua result de dua khach vao nha G (VO LUONG KHONG XU) hay nha S (PHUC MA NGU CHUA)
			if (result % 2 != 0) // Vao nha G
			{
				cout << "Welcome to GOJO Restaurant!" << endl;
				this->gojoRestaurant->welcomeToGojo(result, ID);
			}
			else // Vao nha S
			{
				cout << "Welcome to Sukana Restaurant!" << endl;
				cusNode *temp = new cusNode(result, ID);
				this->sukanaRestaurant->arrayOfAreas[ID - 1]->listCusInArea.push_back(temp);
				this->sukanaRestaurant->welcomeToSukanaRestaurant(ID);
			}
		}
	}
	void KOKUSEN()
	{
		int Y;
		for (int i = 0; i < MAXSIZE; i++)
		{
			if (this->gojoRestaurant->arrayOfRoot[i])
			{
				if (this->gojoRestaurant->countNode(this->gojoRestaurant->arrayOfRoot[i]) == 1)
				{
					Y = this->gojoRestaurant->numberOfOrderArray(this->gojoRestaurant->arrayOfRoot[i]) - 1;
				}
				else
				{
					Y = this->gojoRestaurant->numberOfOrderArray(this->gojoRestaurant->arrayOfRoot[i]) - 1;
				}
			}
			this->gojoRestaurant->getCustomerOut(Y, i + 1);
		}
	}
	void KEITEIKEN(int num)
	{
		vector<sukanaNode *> areaToDelete = this->sukanaRestaurant->minCusArea(num);
		for (int i = 0; i < areaToDelete.size(); i++)
		{
			this->sukanaRestaurant->getOutCus(areaToDelete[i]->areaLabel, num);
		}
	}
	void HAND()
	{
		this->HuffTree->printInOrder(this->HuffmanNodeListToHand[HuffmanNodeListToHand.size() - 1]);
	}
	void LIMITLESS(int num)
	{
		if (num > 0 && num <= MAXSIZE)
		{
			this->gojoRestaurant->printInOrderBST(this->gojoRestaurant->arrayOfRoot[num - 1]); //
		}
	}
	void CLEAVE(int num)
	{
		this->sukanaRestaurant->printPreOderHeap(0, num);
	}
};

void simulate(string filename)
{
	Restaurant *r = new Restaurant();
	ifstream ss(filename);
	string str, name, num, maxsize;

	while (ss >> str)
	{
		if (str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize);
		}
		else if (str == "LAPSE")
		{
			ss >> name;
			r->LAPSE(name);
		}
		else if (str == "KOKUSEN")
		{
			r->KOKUSEN();
		}
		else if (str == "KEITEIKEN")
		{
			ss >> num;
			r->KEITEIKEN(stoi(num));
		}
		else if (str == "HAND")
		{
			r->HAND();
		}
		else if (str == "LIMITLESS")
		{
			ss >> num;
			r->LIMITLESS(stoi(num));
		}
		else
		{
			ss >> num;
			r->CLEAVE(stoi(num));
		}
	}
	delete r;
}