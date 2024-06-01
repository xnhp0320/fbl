#include <stdint.h>
#include <fbl/intrusive_double_list.h>

// An object that holds an immutable int and can exist on a doubly linked list
// managed using a std::unique_ptr
class MyObject : public fbl::DoublyLinkedListable<std::unique_ptr<MyObject>> {
 public:
  explicit MyObject(int val) : val_(val) {}

  int val() const { return val_; }

 private:
  const int val_;
};

void TakeThisObjectFromMe(std::unique_ptr<MyObject>) {}

void DoThings() {
  fbl::DoublyLinkedList<std::unique_ptr<MyObject>> list;

  // Add 100 random integers to our list.
  for (uint32_t i = 0; i < 100; ++i) {
    list.push_back(std::make_unique<MyObject>(rand()));
  }

  // print out any members of the list that are even
  for (const auto& obj : list) {
    if (!(obj.val() % 2)) {
      printf("Even Object %d\n", obj.val());
    }
  }

  // Remove any objects that are divisible by 7 and give them to someone else.
  for (auto iter = list.begin(); iter != list.end(); ) {
    auto consider = iter++;
    if (!(consider->val() % 7)) {
      TakeThisObjectFromMe(list.erase(consider));
    }
  }

  // Destroy the rest of the object by forcing the list to release its unique
  // references to the objects. We could also simply let the list leave the
  // scope of the function, which would do the same.
  list.clear();
}



int main() {
  DoThings();
}
