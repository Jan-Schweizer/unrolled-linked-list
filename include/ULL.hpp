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
// ------------------------------------------------------------------------
// TODO: make BLOCK_SIZE as template parameter
#define BLOCK_SIZE 15
// ------------------------------------------------------------------------
// TODO: Replace data[i] = data[i + 1] with std::copy
// ------------------------------------------------------------------------
template <class V>
class ULL {
   class Node {
      friend class ULL;

      V data[BLOCK_SIZE + 1];
      Node* next = nullptr;
      Node* prev = nullptr;
      size_t length = 0;

      template <class... Args>
      void insert_at(size_t i, Args&&... args);

      template <class... Args>
      void append(Args&&... args) {
         insert_at(length, std::forward<Args>(args)...);
      }

      template <class... Args>
      void prepend(Args&&... args) {
         insert_at(0, std::forward<Args>(args)...);
      }

      void shift_r();

      V remove_at(size_t i);
   };

   struct Location {
      Node* u;
      size_t i;
      Location(Node* u, int i) : u(u), i(i) {}
   };

   public:
   /// Store the end of the list in head
   ///
   /// ->prev.
   Node* head = nullptr; // TODO: Think about using a unique_ptr
   size_t length = 0;
   size_t node_count = 0;

   /// Returns the number of elements in the list.
   size_t get_length() { return length; }

   /// Returns the size of a node in bytes.
   size_t get_node_size() { return BLOCK_SIZE + 1; }

   /// Returns true if the list is empty.
   bool is_empty() { return length == 0; }

   ULL();
   // ~ULL(); // delete every node

   /// Finds value at position i. Returns Location of value.
   Location find_at(int i);

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

   private:
   /// Spreads the elements of the sequence u.next to v onto the sequence u.next to v.next,
   /// such that each node in the sequence u.next to v contains BLOCK_SIZE elements and
   /// v.next contains BLOCK_SIZE - 1 elements.
   void spread(Node* u, Node* v);
};
// Node - Begin
// ------------------------------------------------------------------------
template <class V>
template <class... Args>
void ULL<V>::Node::insert_at(size_t i, Args&&... args) {
   assert(length < BLOCK_SIZE + 1);

   for (size_t idx = length; idx > i; --idx) {
      data[idx] = data[idx - 1];
   }
   new (&data[i]) V(args...);
   ++length;
}
// ------------------------------------------------------------------------
template <class V>
void ULL<V>::Node::shift_r() {
   Node* u = this->next;
   for (size_t idx = u->length; idx > 0; --idx) {
      u->data[idx] = u->data[idx - 1];
   }
   u->data[0] = data[length - 1];
   ++u->length;
   --length;
}
// ------------------------------------------------------------------------
template <class V>
V ULL<V>::Node::remove_at(size_t i) {
   assert(i >= 0 && i < length);

   V ret = data[i];
   for (size_t idx = i; idx < length - 1; ++idx) {
      data[idx] = data[idx + 1];
   }
   --length;
   return ret;
}
// ------------------------------------------------------------------------
// Node - End
// ------------------------------------------------------------------------
// ULL - Begin
template <class V>
ULL<V>::ULL() = default;
// ------------------------------------------------------------------------
template <class V>
class ULL<V>::Location ULL<V>::find_at(int i) {
   Node* u = head;
   if (i < length / 2) { // Start at front of list and search forwards
      while (i >= u->length) {
         i -= u->length;
         u = u->next;
      }
      return Location(u, i);
   } else { // Start at back of list and search backwards
      int n = length;
      while (i < n) {
         u = u->prev;
         n -= u->length;
      }
      return Location(u, i - n);
   }
}
// ------------------------------------------------------------------------
template <class V>
template <class... Args>
void ULL<V>::insert_at(size_t i, Args&&... args) {
   assert(i >= 0 && i <= length);

   if (head == nullptr) {
      head = new Node;
      head->prev = head;
      node_count = 1;
   }

   // Inserting at end of list
   if (i == length) {
      Node* end = head->prev;
      if (end->length == BLOCK_SIZE + 1) {
         end = new Node;
         head->prev->next = end;
         head->prev = end;
         ++node_count;
      }
      end->append(std::forward<Args>(args)...);
      ++length;
      return;
   }

   // Inserting in middle of list
   Location l = find_at(i);
   int r = 0;
   Node* u = l.u;
   while (u != nullptr && r < BLOCK_SIZE && u->length == BLOCK_SIZE + 1) {
      u = u->next;
      ++r;
   }

   if (u == nullptr) { // case 2
      Node* end = new Node;
      head->prev->next = end;
      head->prev = end;
      u = end->prev;
      ++node_count;
   } else if (r == BLOCK_SIZE) { // case 3
      Node* tmp = new Node;
      if (u->next) {
         u->next->prev = tmp;
         tmp->next = u->next;
      } else {
         head->prev = tmp;
         tmp->next = nullptr;
      }
      u->next = tmp;
      tmp->prev = u;
      spread(l.u, u);
      ++node_count;
   } else { // case 1
      u = u->prev;
   }

   while (l.u != u) {
      l.u->shift_r();
      u = u->prev;
   }
   l.u->shift_r();

   l.u->insert_at(l.i, std::forward<Args>(args)...);
   ++length;
}
// ------------------------------------------------------------------------

template <class V>
void ULL<V>::spread(Node* u, Node* v) {
   size_t offset = length - BLOCK_SIZE + 1;
   std::copy(
      std::begin(v->data) + offset,
      std::begin(v->data) + offset + BLOCK_SIZE - 1,
      std::begin(v->next->data));
   length -= BLOCK_SIZE - 1;
   v->next->length += BLOCK_SIZE - 1;

   v = v->prev;

   while (v != u) {
      while (v->next->length < BLOCK_SIZE) {
         v->shift_r();
      }
      v = v->prev;
   }
}
// ------------------------------------------------------------------------
// Node - End
#endif //UNROLLED_LINKED_LIST_ULL_HPP
