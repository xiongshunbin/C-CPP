#include <iostream>
#include <vector>

// 页 [大块内存] -> 切分 -> [小块1] [小块2] ...... [小块n]
//						↑			↓
//					allocate	deallocate
// 
// [大块内存] -> 切分 -> [小块1] [小块2] ...... [小块n]

static inline std::size_t align_up(std::size_t n, std::size_t align)
{
	return (n + (align - 1)) & ~(align - 1);
}

class FixedSizePool
{
public:
	explicit FixedSizePool(std::size_t block_size,
						   std::size_t blocks_per_page = 1024)
	{
		blocks_per_page_ = blocks_per_page;
		block_size_ = adjust_block_size(block_size);
		free_list_ = nullptr;
	}

	~FixedSizePool()
	{
		for (void* p : pages_)
		{
			::operator delete[](p);
		}
	}

	// 分配一小块内存
	void* allocate()
	{
		if (free_list_ == nullptr)
		{
			expand();
		}

		Node* head = free_list_;
		free_list_ = head->next;
		return head;
	}

	// 归还小块内存
	void deallocate(void* p)
	{
		if (p == nullptr)
		{
			return;
		}
		Node* node = static_cast<Node*>(p);
		node->next = free_list_;
		free_list_ = node;
	}

	std::size_t block_size() const { return block_size_; }
	std::size_t blocks_per_page() const { return blocks_per_page_; }
	std::size_t pages_size() const { return pages_.size(); }

private:
	// 内存对齐
	std::size_t adjust_block_size(std::size_t s)
	{
		std::size_t min = sizeof(void*);
		std::size_t ret = align_up(s < min ? min : s, alignof(void*));
		return ret;
	}

	// 每次向系统申请一页的内存, 并把这一页的内存切为很多小块, 然后把这些小块挂到空闲链表中
	void expand()
	{
		// 一整页内存的字节数
		std::size_t page_bytes = block_size_ * blocks_per_page_;
		char* page = static_cast<char*>(::operator new[](page_bytes));
		pages_.push_back(page);

		// 把一整页的内存分成 blocks_per_page_ 个节点, 串成 free_list_
		for (std::size_t i = 0; i < blocks_per_page_; i++)
		{
			char* addr = page + i * block_size_;
			Node* node = reinterpret_cast<Node*>(addr);
			node->next = free_list_;
			free_list_ = node;
		}
	}

private:
	struct Node { Node* next; };
	std::size_t block_size_;
	std::size_t blocks_per_page_;

	Node* free_list_;				// 空闲小块的单向链表头
	std::vector<void*> pages_;		// 所有已经分配的"页" - 析构统一释放
};

// =================== 应用示例 ===================

struct Partical
{
	float x, y, z;
	int life;

	static void* operator new(std::size_t n);
	static void operator delete(void* p) noexcept;
	void update() { ++life; }
};

static FixedSizePool g_partical_pool(sizeof(Partical), 4096);

void* Partical::operator new(std::size_t n)
{
	return g_partical_pool.allocate();
}

void Partical::operator delete(void* p) noexcept
{
	g_partical_pool.deallocate(p);
}

int main()
{
	std::vector<Partical*> vec;
	vec.reserve(10000);

	for (int i = 0; i < 10000; i++)
	{
		Partical* p = new Partical{ 0, 0, 0, 0 };
		vec.push_back(p);
	}

	for (auto* p : vec)
	{
		delete p;
	}

	std::cout << "BlockSize = " << g_partical_pool.block_size()
		<< ", BlocksPerPage = " << g_partical_pool.blocks_per_page()
		<< ", PagesSize = " << g_partical_pool.pages_size()
		<< std::endl;

	return 0;
}