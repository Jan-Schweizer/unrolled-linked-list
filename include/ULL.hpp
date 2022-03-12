#ifndef UNROLLED_LINKED_LIST_ULL_HPP
#define UNROLLED_LINKED_LIST_ULL_HPP
// ------------------------------------------------------------------------
/*
 * https://opendatastructures.org/ods-java/3_3_SEList_Space_Efficient_.html
 */
// ------------------------------------------------------------------------
#include <cassert>
#include <cstdint>
// ------------------------------------------------------------------------
#define L3_SIZE (1 << 22) // 4 MiB2
#define BLOCK_SIZE 16
// ------------------------------------------------------------------------
template <class V>
class ULL {
   class Node {
      friend class ULL;

      V data[BLOCK_SIZE];
      Node* next = nullptr;
      Node* prev = nullptr;
      size_t length = 0;

      template <class... Args>
      void add(Args&&... args);
   };

   public:
   Node* head = nullptr;
   size_t length;
   size_t node_count;

   /// Returns the number of elements in the list
   size_t get_length() { return length; }

   /// Returns the size of a node in bytes
   size_t get_node_size() { return BLOCK_SIZE; }

   /// Returns true if the list is empty
   bool is_empty() { return length == 0; }

   ULL();
   // ~ULL(); // delete every node

   /// Inserts a value at position i into the list
   template <class... Args>
   void insert_at(size_t i, Args&&... args);

   /// Appends a value at the end of the list
   template <class... Args>
   void append(Args&&... args) {
      return insert_at(length, std::forward<Args>(args)...);
   }

   /// Prepends a value at the front of the list
   template <class... Args>
   void prepend(Args&&... args) {
      return insert_at(0, std::forward<Args>(args)...);
   }
};
// Node - Begin
// ------------------------------------------------------------------------
template <class V>
template <class... Args>
void ULL<V>::Node::add(Args&&... args) {
   new (&data[length]) V(args...);
   ++length;
}
// Node - End
// ------------------------------------------------------------------------
// ULL - Begin
template <class V>
ULL<V>::ULL() : length(0), node_count(1) {}
// ------------------------------------------------------------------------
template <class V>
template <class... Args>
void ULL<V>::insert_at(size_t i, Args&&... args) {
   assert(i >= 0 && i <= length);

   if (i == length) {
      if (head == nullptr || head->length == BLOCK_SIZE) {
         Node* tmp = head;
         head = new Node;
         head->next = tmp;
      }
   }

   head->add(std::forward<Args>(args)...);
   ++length;
}
// ------------------------------------------------------------------------
// Node - End
#endif //UNROLLED_LINKED_LIST_ULL_HPP
