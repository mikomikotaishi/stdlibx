import stdx;

using stdx::io::Scanner;

#ifdef __GNUC__
using namespace stdx::core;
#endif

int main() {
    Scanner in(System::in);
    System::out.println("Enter some text:");
    Optional<String> line = in.next_line();
    System::out.println("You entered: {}", line.value_or("nothing"));
}
