/* BinaryTree
 * A binary tree data structure
 */

public struct BinaryTree<T> {
    private Node* rootNode;

    public struct Node<T> {
        T* data;
        Node* left;
        Node* right;
    }
}
