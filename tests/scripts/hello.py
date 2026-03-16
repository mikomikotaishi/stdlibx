import sys

def main() -> None:
    print("Hello from python")
    for arg in sys.argv[1:]:
        print(f"arg: {arg}")
    if "--stderr" in sys.argv:
        print("This goes to stderr", file=sys.stderr)
    if "--exit-code" in sys.argv:
        idx: int = sys.argv.index("--exit-code")
        sys.exit(int(sys.argv[idx + 1]))

if __name__ == "__main__":
    main()
