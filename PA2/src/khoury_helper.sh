#!/bin/bash

# Khoury Linux Cluster Helper Script
# This script helps run experiments on the Khoury cluster across different machines

echo "=== Khoury Cluster Helper ==="
echo ""

# Configuration
PROJECT_DIR="$HOME/robot_factory"
PORT=12345
ORDERS=10000

print_usage() {
    echo "Usage: $0 [command] [options]"
    echo ""
    echo "Commands:"
    echo "  setup             - Initial setup and compilation"
    echo "  server [machine] [experts] - Start server on specified machine"
    echo "  client [server_machine] [customers] [type] - Run client"
    echo "  experiment [exp_num] - Run specific experiment (1-4)"
    echo "  get_ip [machine]  - Get IP of specified machine"
    echo "  cleanup           - Kill all running servers"
    echo ""
    echo "Examples:"
    echo "  $0 setup"
    echo "  $0 server linux-071 2"
    echo "  $0 client linux-071 16 0"
    echo "  $0 get_ip linux-071"
    echo ""
}

# Get IP address of a machine
get_machine_ip() {
    local machine=$1
    if [ -z "$machine" ]; then
        echo "Error: Machine name required"
        return 1
    fi
    
    echo "Getting IP for $machine..."
    ssh "$machine.khoury.northeastern.edu" "hostname -i" 2>/dev/null
}

# Setup project on cluster
setup_project() {
    echo "Setting up project..."
    
    # Create project directory
    mkdir -p "$PROJECT_DIR"
    cd "$PROJECT_DIR" || exit 1
    
    echo "Project directory: $PROJECT_DIR"
    echo ""
    echo "Next steps:"
    echo "1. Copy your source files to: $PROJECT_DIR"
    echo "2. Run: cd $PROJECT_DIR && make all"
    echo "3. Choose two machines for server and client"
    echo "4. Run experiments"
    echo ""
}

# Start server on specified machine
start_server() {
    local machine=$1
    local experts=${2:-0}
    
    if [ -z "$machine" ]; then
        echo "Error: Machine name required"
        echo "Usage: $0 server [machine] [experts]"
        return 1
    fi
    
    echo "Starting server on $machine with $experts experts..."
    echo "Port: $PORT"
    echo ""
    
    # Get machine's IP
    local ip=$(get_machine_ip "$machine")
    echo "Server IP: $ip"
    echo ""
    
    echo "Command to run on $machine:"
    echo "  cd $PROJECT_DIR && ./server $PORT $experts"
    echo ""
    echo "To start server, either:"
    echo "1. SSH manually: ssh $machine.khoury.northeastern.edu"
    echo "2. Or run in background (note: may disconnect if terminal closes)"
    echo ""
    
    read -p "Start server now? (y/n) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        ssh "$machine.khoury.northeastern.edu" "cd $PROJECT_DIR && ./server $PORT $experts"
    fi
}

# Run client
run_client() {
    local server_machine=$1
    local customers=${2:-16}
    local robot_type=${3:-0}
    
    if [ -z "$server_machine" ]; then
        echo "Error: Server machine name required"
        echo "Usage: $0 client [server_machine] [customers] [robot_type]"
        return 1
    fi
    
    echo "Getting server IP from $server_machine..."
    local server_ip=$(get_machine_ip "$server_machine")
    
    if [ -z "$server_ip" ]; then
        echo "Error: Could not get IP for $server_machine"
        return 1
    fi
    
    echo "Server IP: $server_ip"
    echo "Customers: $customers"
    echo "Orders per customer: $ORDERS"
    echo "Robot type: $robot_type"
    echo ""
    
    echo "Running client..."
    cd "$PROJECT_DIR" || exit 1
    ./client "$server_ip" "$PORT" "$customers" "$ORDERS" "$robot_type"
}

# Run full experiment
run_experiment() {
    local exp_num=$1
    
    if [ -z "$exp_num" ]; then
        echo "Error: Experiment number required (1-4)"
        return 1
    fi
    
    echo "=== Running Experiment $exp_num ==="
    echo ""
    echo "This requires TWO separate terminal windows:"
    echo "  Terminal 1: Server machine"
    echo "  Terminal 2: Client machine"
    echo ""
    
    case $exp_num in
        1)
            echo "EXPERIMENT 1: Regular Robots (No Experts)"
            echo ""
            echo "Terminal 1 - Start server:"
            echo "  $0 server linux-071 0"
            echo ""
            echo "Terminal 2 - Run clients with varying customers:"
            for customers in 1 2 4 8 16 32 64 128 256; do
                echo "  $0 client linux-071 $customers 0"
            done
            ;;
        2)
            echo "EXPERIMENT 2: Special Robots (1 Expert)"
            echo ""
            echo "Terminal 1 - Start server:"
            echo "  $0 server linux-071 1"
            echo ""
            echo "Terminal 2 - Run clients with varying customers:"
            for customers in 1 2 4 8 16 32 64 128 256; do
                echo "  $0 client linux-071 $customers 1"
            done
            ;;
        3)
            echo "EXPERIMENT 3: Special Robots (16 Experts)"
            echo ""
            echo "Terminal 1 - Start server:"
            echo "  $0 server linux-071 16"
            echo ""
            echo "Terminal 2 - Run clients with varying customers:"
            for customers in 1 2 4 8 16 32 64 128 256; do
                echo "  $0 client linux-071 $customers 1"
            done
            ;;
        4)
            echo "EXPERIMENT 4: Special Robots (Matching Experts)"
            echo ""
            echo "For each customer count, restart server with matching experts:"
            for customers in 1 2 4 8 16 32 64 128 256; do
                echo ""
                echo "Run $customers customers:"
                echo "  Terminal 1: $0 server linux-071 $customers"
                echo "  Terminal 2: $0 client linux-071 $customers 1"
            done
            ;;
        *)
            echo "Error: Invalid experiment number (must be 1-4)"
            return 1
            ;;
    esac
    echo ""
}

# Cleanup running servers
cleanup() {
    echo "Cleaning up running servers..."
    
    echo "Checking local machine..."
    pkill -f "./server" 2>/dev/null && echo "  Killed local server" || echo "  No local server running"
    
    echo ""
    echo "To cleanup on remote machine:"
    echo "  ssh linux-071.khoury.northeastern.edu 'pkill -f \"./server\"'"
    echo ""
}

# Check cluster status
check_status() {
    echo "Checking project status..."
    echo ""
    
    if [ -d "$PROJECT_DIR" ]; then
        echo "✓ Project directory exists: $PROJECT_DIR"
        
        cd "$PROJECT_DIR" || exit 1
        
        if [ -f "client" ]; then
            echo "✓ Client binary found"
        else
            echo "✗ Client binary not found (run: make client)"
        fi
        
        if [ -f "server" ]; then
            echo "✓ Server binary found"
        else
            echo "✗ Server binary not found (run: make server)"
        fi
        
        if [ -d "experiment_results" ]; then
            echo "✓ Results directory exists"
            local file_count=$(ls experiment_results/*.tsv 2>/dev/null | wc -l)
            echo "  Found $file_count data files"
        else
            echo "○ No results yet"
        fi
    else
        echo "✗ Project directory not found"
        echo "  Run: $0 setup"
    fi
    
    echo ""
    echo "Current machine: $(hostname)"
    echo "Current IP: $(hostname -i)"
    echo ""
}

# Available machines
list_machines() {
    echo "Available Khoury Linux machines:"
    echo ""
    echo "  linux-071.khoury.northeastern.edu"
    echo "  linux-072.khoury.northeastern.edu"
    echo "  linux-073.khoury.northeastern.edu"
    echo "  linux-074.khoury.northeastern.edu"
    echo "  linux-075.khoury.northeastern.edu"
    echo "  linux-076.khoury.northeastern.edu"
    echo "  linux-077.khoury.northeastern.edu"
    echo "  linux-078.khoury.northeastern.edu"
    echo "  linux-079.khoury.northeastern.edu"
    echo "  linux-080.khoury.northeastern.edu"
    echo "  linux-081.khoury.northeastern.edu"
    echo "  linux-082.khoury.northeastern.edu"
    echo "  linux-083.khoury.northeastern.edu"
    echo "  linux-084.khoury.northeastern.edu"
    echo "  linux-085.khoury.northeastern.edu"
    echo ""
    echo "Usage: ssh linux-071.khoury.northeastern.edu"
    echo ""
}

# Main command dispatcher
case "$1" in
    setup)
        setup_project
        ;;
    server)
        start_server "$2" "$3"
        ;;
    client)
        run_client "$2" "$3" "$4"
        ;;
    experiment)
        run_experiment "$2"
        ;;
    get_ip)
        get_machine_ip "$2"
        ;;
    cleanup)
        cleanup
        ;;
    status)
        check_status
        ;;
    machines)
        list_machines
        ;;
    *)
        if [ -z "$1" ]; then
            check_status
        else
            echo "Error: Unknown command '$1'"
            echo ""
        fi
        print_usage
        exit 1
        ;;
esac

# Make executable: chmod +x khoury_helper.sh