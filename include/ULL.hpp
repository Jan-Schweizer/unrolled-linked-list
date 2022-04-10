#ifndef UNROLLED_LINKED_LIST_ULL_HPP
#define UNROLLED_LINKED_LIST_ULL_HPP
// ------------------------------------------------------------------------
/*
 * https://opendatastructures.org/ods-java/3_3_SEList_Space_Efficient_.html
 */
// ------------------------------------------------------------------------
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
// ------------------------------------------------------------------------
// TODO: Replace data[i] = data[i + 1] with std::copy
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE = 3>
class ULL {
   class Node {
      friend class ULL;

      V data[BLOCK_SIZE + 1];
      Node* next = nullptr;
      Node* prev = nullptr;
      size_t size = 0;

      template <class... Args>
      void insert_at(size_t i, Args&&... args);

      template <class... Args>
      void append(Args&&... args) {
         insert_at(size, std::forward<Args>(args)...);
      }

      template <class... Args>
      void prepend(Args&&... args) {
         insert_at(0, std::forward<Args>(args)...);
      }

      bool is_empty() { return size == 0; }

      void shift_r();
      void shift_l();
      void remove_at(size_t i);
   };

   struct Location {
      Node* u;
      size_t i;
      Location(Node* u, int i) : u(u), i(i) {}
   };

   public:
   size_t length = 0;
   size_t node_count = 0;

   /// Returns the size of a node.
   size_t get_node_size() { return BLOCK_SIZE + 1; }

   /// Returns the BLOCK_SIZE
   size_t get_block_size() { return BLOCK_SIZE; }

   /// Returns true if the list is empty.
   bool is_empty() { return length == 0; }

   ULL();
   ~ULL();

   /// Finds value at position i. Returns Location of value.
   Location find_at(int i);

   /// Gets a value at position i. Returns a pointer the value or nullptr if position is out of range.
   V* get(int i) {
      if (i < 0 || i >= length) return nullptr;
      Location l = find_at(i);
      return &l.u->data[l.i];
   }

   /// Inserts a value at position i into the list.
   template <class... Args>
   void insert_at(size_t i, Args&&... args);

   /// Appends a value at the end of the list.
   template <class... Args>
   void append(Args&&... args) {
      return insert_at(length, std::forward<Args>(args)...);
   }

   /// Prepends a value at the front of the list.
   template <class... Args>
   void prepend(Args&&... args) {
      return insert_at(0, std::forward<Args>(args)...);
   }

   /// Removes an element at position i.
   void remove_at(size_t i);

   /// Removes the last element of the list.
   void pop_back() { remove_at(length - 1); }

   /// Removes the first element of the list.
   void pop_front() { remove_at(0); }

   /// Prints the list to cout.
   void print_list();

   /// Returns a reference to the element at specified position i. No bounds checking is performed.
   V& operator[](size_t i);

   struct Iterator {
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type = V;
      using difference_type = std::ptrdiff_t;
      using pointer = V*;
      using reference = V&;

      Iterator(Node* node, size_t i, ULL<V, BLOCK_SIZE>& ull) : node_(node), i_(i), ull_(ull) {}

      reference operator*() const { return node_->data[i_]; }
      pointer operator->() const { return &node_->data[i_]; }
      reference operator[](size_t i) const { // Not needed for bidirectional iterator
         auto l = ull_.find_at(i);
         return l.u->data[l.i];
      }
      Iterator& operator++() {
         if (i_ == node_->size - 1) {
            node_ = node_->next;
            i_ = 0;
         } else {
            ++i_;
         }
         return *this;
      }
      Iterator operator++(int) {
         Iterator tmp = *this;
         ++(*this);
         return tmp;
      }
      Iterator& operator--() {
         if (i_ == 0) {
            if (node_ == ull_.head) {
               node_ = nullptr; // Needed because end of list is stored in head.prev
            } else {
               node_ = node_->prev;
               i_ = node_->size - 1;
            }
         } else {
            --i_;
         }
         return *this;
      }
      Iterator operator--(int) {
         Iterator tmp = *this;
         --(*this);
         return tmp;
      }
      bool operator==(const Iterator& other) const { return node_ == other.node_ && i_ == other.i_; }
      bool operator!=(const Iterator& other) const { return node_ != other.node_ || i_ != other.i_; }

      private:
      Node* node_;
      size_t i_;
      ULL<V, BLOCK_SIZE>& ull_; // Needed to identify the end of list in head.prev
   };

   Iterator begin() { return Iterator(head, 0, *this); }
   Iterator end() { return Iterator(nullptr, 0, *this); }

   private:
   /// Store the end of the list in head->prev.
   Node* head = nullptr;

   /// Spreads the elements of the sequence u.next to v onto the sequence u.next to v.next,
   /// such that each node in the sequence u.next to v contains BLOCK_SIZE elements and
   /// v.next contains BLOCK_SIZE - 1 elements.
   void spread(Node* u, Node* v);

   /// Gathers the elements of the sequence u to u + BLOCK_SIZE - 1 onto the sequence u to
   /// u + BLOCK_SIZE - 2, such that each node in the sequence u to u + BLOCK_SIZE - 2 contains
   /// BLOCK_SIZE elements. The last node of the original sequence, which is now empty, is removed
   void gather(Node* u);
};
// Node - Begin
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
template <class... Args>
void ULL<V, BLOCK_SIZE>::Node::insert_at(size_t i, Args&&... args) {
   assert(size < BLOCK_SIZE + 1);

   for (size_t idx = size; idx > i; --idx) {
      data[idx] = data[idx - 1];
   }
   new (&data[i]) V(args...);
   ++size;
}
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
void ULL<V, BLOCK_SIZE>::Node::shift_r() {
   Node* u = next;
   for (size_t idx = u->size; idx > 0; --idx) {
      u->data[idx] = u->data[idx - 1];
   }
   u->data[0] = data[size - 1];
   ++u->size;
   --size;
}
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
void ULL<V, BLOCK_SIZE>::Node::remove_at(size_t i) {
   assert(i >= 0 && i < size);

   for (size_t idx = i; idx < size - 1; ++idx) {
      data[idx] = data[idx + 1];
   }
   --size;
}
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
void ULL<V, BLOCK_SIZE>::Node::shift_l() {
   Node* u = next;
   data[size] = u->data[0];
   for (size_t idx = 0; idx < u->size - 1; ++idx) {
      u->data[idx] = u->data[idx + 1];
   }
   --u->size;
   ++size;
}
// ------------------------------------------------------------------------
// Node - End
// ------------------------------------------------------------------------
// ULL - Begin
template <class V, size_t BLOCK_SIZE>
ULL<V, BLOCK_SIZE>::ULL() = default;
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
ULL<V, BLOCK_SIZE>::~ULL() {
   Node* current = head;
   Node* next;
   while (current) {
      for (size_t i = 0; i < current->size; ++i) {
         current->data[i].~V();
      }
      next = current->next;
      delete current;
      current = next;
   }
}
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
class ULL<V, BLOCK_SIZE>::Location ULL<V, BLOCK_SIZE>::find_at(int i) {
   Node* u = head;
   if (i < length / 2) { // Start at front of list and search forwards
      while (i >= u->size) {
         i -= u->size;
         u = u->next;
      }
      return Location(u, i);
   } else { // Start at back of list and search backwards
      int n = length;
      while (i < n) {
         u = u->prev;
         n -= u->size;
      }
      return Location(u, i - n);
   }
}
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
template <class... Args>
void ULL<V, BLOCK_SIZE>::insert_at(size_t i, Args&&... args) {
   assert(i >= 0 && i <= length);

   if (head == nullptr) {
      head = new Node;
      head->prev = head;
      node_count = 1;
   }

   // Inserting at end of list
   if (i == length) {
      Node* end = head->prev;
      if (end->size == BLOCK_SIZE + 1) {
         end = new Node;
         head->prev->next = end;
         end->prev = head->prev;
         head->prev = end;
         ++node_count;
      }
      end->append(std::forward<Args>(args)...);
      ++length;
      return;
   }

   // Inserting not at end of list
   Location l = find_at(i);
   int r = 0;
   Node* u = l.u;
   while (u != nullptr && r < BLOCK_SIZE && u->size == BLOCK_SIZE + 1) {
      u = u->next;
      ++r;
   }

   if (u == nullptr) { // case 2
      Node* end = new Node;
      head->prev->next = end;
      end->prev = head->prev;
      head->prev = end;
      u = end->prev;
      ++node_count;
   } else if (r == BLOCK_SIZE) { // case 3
      u = u->prev;
      Node* tmp = new Node;
      u->next->prev = tmp;
      tmp->next = u->next;
      u->next = tmp;
      tmp->prev = u;
      ++node_count;
      spread(l.u, u);
   } else if (l.u != u) { // case 1
      u = u->prev;
   }

   while (l.u != u) {
      u->shift_r();
      u = u->prev;
   }
   if (l.u->size == BLOCK_SIZE + 1) l.u->shift_r();

   l.u->insert_at(l.i, std::forward<Args>(args)...);
   ++length;
}
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
void ULL<V, BLOCK_SIZE>::spread(Node* u, Node* v) {
   // Bulk copy the first BLOCK_SIZE - 1 elements from vo to (the empty) v.next in order to save on shifting
   size_t offset = v->size - BLOCK_SIZE + 1;
   std::copy(
      std::begin(v->data) + offset,
      std::begin(v->data) + offset + BLOCK_SIZE - 1,
      std::begin(v->next->data));
   v->size -= BLOCK_SIZE - 1;
   v->next->size = BLOCK_SIZE - 1;

   v = v->prev;

   while (v != u) {
      while (v->next->size < BLOCK_SIZE) {
         v->shift_r();
      }
      v = v->prev;
   }
}
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
void ULL<V, BLOCK_SIZE>::remove_at(size_t i) {
   Location l = find_at(i);

   int r = 0;
   Node* u = l.u;
   while (u != nullptr && r < BLOCK_SIZE && u->size == BLOCK_SIZE - 1) {
      u = u->next;
      ++r;
   }

   if (r == BLOCK_SIZE && u != nullptr) {
      gather(l.u);
   }

   u = l.u;
   u->remove_at(l.i);

   while (u->next != nullptr && u->size < BLOCK_SIZE - 1) {
      u->shift_l();
      u = u->next;
   }

   if (u->is_empty()) {
      head->prev = u->prev;
      u->prev->next = nullptr;
      if (u == head) {
         head = nullptr;
      }
      delete u;
   }

   --length;
}
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
void ULL<V, BLOCK_SIZE>::gather(Node* u) {
   for (size_t i = 0; i < BLOCK_SIZE - 1; ++i) {
      while (u->size < BLOCK_SIZE) {
         u->shift_l();
      }
      u = u->next;
   }

   u->prev->next = u->next;
   u->next->prev = u->prev;
   delete u;
}
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
void ULL<V, BLOCK_SIZE>::print_list() {
   Node* current = head;
   while (current != nullptr) {
      std::cout << "[";
      for (size_t i = 0; i < current->size - 1; ++i) {
         std::cout << current->data[i] << ", ";
      }
      std::cout << current->data[current->size - 1] << "]"
                << " -> " << std::endl;
      current = current->next;
   }
   std::cout << "null" << std::endl;
}
// ------------------------------------------------------------------------
template <class V, size_t BLOCK_SIZE>
V& ULL<V, BLOCK_SIZE>::operator[](size_t i) {
   assert(i >= 0 && i < length);

   auto l = find_at(i);
   return l.u->data[l.i];
}
// ------------------------------------------------------------------------
// ULL - End
#endif //UNROLLED_LINKED_LIST_ULL_HPP
