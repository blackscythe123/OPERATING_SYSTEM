import java.util.ArrayList;

public class GenericStack {
    // GenericStack class
    static class GenericStack<T> {
        private ArrayList<T> stack;
        private int top;

        public GenericStack() {
            stack = new ArrayList<>();
            top = -1;
        }

        public void push(T element) {
            stack.add(element);
            top++;
        }

        public T pop() {
            if (isEmpty()) {
                throw new IllegalStateException("Stack is empty");
            }
            T element = stack.remove(top);
            top--;
            return element;
        }

        public T peek() {
            if (isEmpty()) {
                throw new IllegalStateException("Stack is empty");
            }
            return stack.get(top);
        }

        public boolean isEmpty() {
            return top == -1;
        }

        public int size() {
            return top + 1;
        }
    }

    // Student class for testing
    static class Student {
        private int id;
        private String name;

        public Student(int id, String name) {
            this.id = id;
            this.name = name;
        }

        @Override
        public String toString() {
            return "Student[ID=" + id + ", Name=" + name + "]";
        }
    }

    // Main method to test GenericStack
    public static void main(String[] args) {
        // Integer Stack Test
        System.out.println("Integer Stack Operations:");
        GenericStack<Integer> intStack = new GenericStack<>();
        intStack.push(10);
        intStack.push(20);
        intStack.push(30);
        System.out.println("Pushing: 10, 20, 30");
        System.out.println("Size: " + intStack.size());
        System.out.println("Is Empty: " + intStack.isEmpty());
        System.out.println("Top element: " + intStack.peek());
        System.out.println("Popped: " + intStack.pop());
        System.out.println("Size after pop: " + intStack.size());

        // String Stack Test
        System.out.println("\nString Stack Operations:");
        GenericStack<String> strStack = new GenericStack<>();
        strStack.push("Hello");
        strStack.push("World");
        System.out.println("Pushing: Hello, World");
        System.out.println("Size: " + strStack.size());
        System.out.println("Top element: " + strStack.peek());
        System.out.println("Popped: " + strStack.pop());
        System.out.println("Size after pop: " + strStack.size());

        // Student Stack Test
        System.out.println("\nStudent Stack Operations:");
        GenericStack<Student> studentStack = new GenericStack<>();
        studentStack.push(new Student(1, "John"));
        studentStack.push(new Student(2, "Jane"));
        System.out.println("Pushing: Student[ID=1, Name=John], Student[ID=2, Name=Jane]");
        System.out.println("Size: " + studentStack.size());
        System.out.println("Top element: " + studentStack.peek());
        System.out.println("Popped: " + studentStack.pop());
        System.out.println("Size after pop: " + studentStack.size());

        // Empty Stack Test
        System.out.println("\nEmpty Stack Test:");
        GenericStack<Integer> emptyStack = new GenericStack<>();
        try {
            emptyStack.pop();
        } catch (IllegalStateException e) {
            System.out.println("Exception: " + e.getMessage());
        }
    }
}