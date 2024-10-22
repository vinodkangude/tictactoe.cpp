#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

struct Book
{
    string title;
    string author;
    string ISBN;
    bool isAvailable;
};

struct Borrower
{
    string name;
    string borrowedISBN;
    time_t borrowDate;
};

vector<Book> books;
vector<Borrower> borrowers;

void addBook(const string &title, const string &author, const string &ISBN)
{
    books.push_back({title, author, ISBN, true});
}

void searchBook()
{
    string keyword;
    cout << "Enter title, author, or ISBN to search: ";
    cin.ignore();
    getline(cin, keyword);

    bool found = false;
    for (const auto &book : books)
    {
        if (book.title == keyword || book.author == keyword || book.ISBN == keyword)
        {
            cout << "Book Found: " << book.title << " by " << book.author << " (ISBN: " << book.ISBN << ")\n";
            cout << "Status: " << (book.isAvailable ? "Available" : "Checked out") << "\n\n";
            found = true;
        }
    }

    if (!found)
    {
        cout << "No book found with that information.\n";
    }
}

void checkoutBook()
{
    string ISBN, borrowerName;
    cout << "Enter ISBN of the book to check out: ";
    cin >> ISBN;

    for (auto &book : books)
    {
        if (book.ISBN == ISBN && book.isAvailable)
        {
            cout << "Enter the borrower's name: ";
            cin.ignore();
            getline(cin, borrowerName);

            book.isAvailable = false;
            time_t now = time(0);

            borrowers.push_back({borrowerName, ISBN, now});
            cout << "Book successfully checked out to " << borrowerName << "!\n";
            return;
        }
    }

    cout << "Book is either not available or does not exist.\n";
}

void returnBook()
{
    string ISBN;
    cout << "Enter ISBN of the book to return: ";
    cin >> ISBN;

    auto borrowerIt = find_if(borrowers.begin(), borrowers.end(), [&](Borrower &b)
                              { return b.borrowedISBN == ISBN; });

    if (borrowerIt != borrowers.end())
    {
        time_t now = time(0);

        double daysBorrowed = difftime(now, borrowerIt->borrowDate) / (60 * 60 * 24);

        cout << "Borrower: " << borrowerIt->name << "\n";
        if (daysBorrowed > 14)
        {
            double fine = (daysBorrowed - 14) * 1.00;
            cout << "Overdue! Days borrowed: " << daysBorrowed << ". Fine: $" << fine << "\n";
        }
        else
        {
            cout << "Book returned on time. No fine.\n";
        }

        for (auto &book : books)
        {
            if (book.ISBN == ISBN)
            {
                book.isAvailable = true;
                break;
            }
        }

        borrowers.erase(borrowerIt);
        cout << "Book returned successfully!\n";
    }
    else
    {
        cout << "Book return failed. The book was not checked out.\n";
    }
}

void calculateFines()
{
    time_t now = time(0);

    cout << "\nOverdue Fines:\n";
    for (const auto &borrower : borrowers)
    {
        double daysBorrowed = difftime(now, borrower.borrowDate) / (60 * 60 * 24);

        if (daysBorrowed > 14)
        {
            double fine = (daysBorrowed - 14) * 1.00;
            cout << "Borrower: " << borrower.name << ", Days Borrowed: " << daysBorrowed << ", Fine: $" << fine << "\n";
        }
    }
}

int main()
{
    int choice;

    addBook("The wings of fire", "APJ abdul kalam", "123456789");
    addBook("chhava", "shivaji sawant", "987654321");
    addBook("shyamachi aai", "sane guruji", "212121411");
    addBook("phakira", "aanabhau sathe", "122111133");
    addBook("why i am an atheist and other works", "bhagat shingh", "7666695369");

    do
    {
        cout << "\nLibrary Management System\n";
        cout << "1. Search for a book\n";
        cout << "2. Check out a book\n";
        cout << "3. Return a book\n";
        cout << "4. Calculate fines for overdue books\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            searchBook();
            break;
        case 2:
            checkoutBook();
            break;
        case 3:
            returnBook();
            break;
        case 4:
            calculateFines();
            break;
        case 5:
            cout << "Exiting the library management system.\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
