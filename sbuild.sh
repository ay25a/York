#!/usr/bin/env bash

set -e

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

usage() {
    echo "Usage:"
    echo "  $0 clean"
    echo "  $0 build <debug|release|relwithdebinfo>"
    echo "  $0 enable <debug|release|relwithdebinfo>"
    echo "  $0 test <debug|release|relwithdebinfo>"
    echo "  $0 run <debug|release|relwithdebinfo>"
    exit 1
}

if [[ $# -lt 1 ]]; then
    usage
fi

COMMAND="$1"

case "$COMMAND" in
    clean)
        if [[ $# -ne 1 ]]; then
            usage
        fi

        rm -rf "$ROOT/bin" "$ROOT/build"
        ;;

    build|enable|run|test)
        if [[ $# -ne 2 ]]; then
            usage
        fi

        CONFIG="$2"

        case "$CONFIG" in
            debug)
                SANDBOX="sandbox_debug"
                ETEST="york_tests_debug"
                ;;
            release)
                SANDBOX="sandbox_release"
                ETEST="york_tests_release"
                ;;
            relwithdebinfo)
                SANDBOX="sandbox_relwithdebinfo"
                ETEST="york_tests_relwithdebinfo"
                ;;
            *)
                usage
                ;;
        esac

        case "$COMMAND" in
            enable)
                ln -sf \
                    "$CONFIG/compile_commands.json" \
                    "$ROOT/build/compile_commands.json"
                ;;

            build)
                cmake --build "$ROOT/build/$CONFIG" -j 8
                ;;

            run)
                cmake --build "$ROOT/build/$CONFIG" -j 8
                cd "$ROOT/bin"
                exec "./$SANDBOX"
                ;;

            test)
                cmake --build "$ROOT/build/$CONFIG" -j 8
                cd "$ROOT/bin"
                exec "./$ETEST"
                ;;
        esac
        ;;

    *)
        usage
        ;;
esac
