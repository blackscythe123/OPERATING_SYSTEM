import java.util.ArrayList;

public class ShapeBox {
    // Abstract Shape class
    abstract static class Shape {
        abstract double getArea();
    }

    // Circle class
    static class Circle extends Shape {
        private double radius;

        public Circle(double radius) {
            this.radius = radius;
        }

        @Override
        double getArea() {
            return Math.PI * radius * radius;
        }
    }

    // Rectangle class
    static class Rectangle extends Shape {
        private double length;
        private double width;

        public Rectangle(double length, double width) {
            this.length = length;
            this.width = width;
        }

        @Override
        double getArea() {
            return length * width;
        }
    }

    // ShapeBox class
    static class ShapeBox<T extends Shape> {
        private ArrayList<T> shapes;

        public ShapeBox() {
            shapes = new ArrayList<>();
        }

        public void addShape(T shape) {
            shapes.add(shape);
        }

        public double calculateTotalArea() {
            double total = 0.0;
            for (T shape : shapes) {
                total += shape.getArea();
            }
            return total;
        }
    }

    // Main method to test ShapeBox
    public static void main(String[] args) {
        // Valid Shapes Test
        System.out.println("ShapeBox Operations:");
        ShapeBox<Shape> shapeBox = new ShapeBox<>();
        shapeBox.addShape(new Circle(5.0));
        System.out.println("Adding Circle with radius 5.0");
        shapeBox.addShape(new Rectangle(4.0, 6.0));
        System.out.println("Adding Rectangle with length 4.0, width 6.0");
        shapeBox.addShape(new Rectangle(16.0, 22.0));
        System.out.println("Adding Rectangle with length 16.0, width 22.0");
        double totalArea = shapeBox.calculateTotalArea();
        System.out.printf("Total area of shapes in the box: %.2f\n", totalArea);

        // Empty ShapeBox Test
        System.out.println("\nEmpty ShapeBox Test:");
        ShapeBox<Shape> emptyBox = new ShapeBox<>();
        System.out.println("Total area: " + emptyBox.calculateTotalArea());
    }
}