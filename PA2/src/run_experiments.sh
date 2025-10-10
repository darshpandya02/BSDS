#!/bin/bash

# Experiment runner for robot factory performance testing
# This script automates running all 4 experiments

# Configuration
SERVER_IP="127.0.0.1"
SERVER_PORT=12345
ORDERS_PER_CUSTOMER=10000

# Output files
OUTPUT_DIR="experiment_results"
mkdir -p $OUTPUT_DIR

# Customer counts to test
CUSTOMER_COUNTS=(1 2 4 8 16 32 64 128 256)

echo "=== Robot Factory Performance Experiments ==="
echo "Server IP: $SERVER_IP"
echo "Server Port: $SERVER_PORT"
echo "Orders per customer: $ORDERS_PER_CUSTOMER"
echo ""

# Function to wait for server to be ready
wait_for_server() {
    echo "Waiting for server to be ready..."
    sleep 2
    
    # Try to connect to server
    for i in {1..10}; do
        if nc -z $SERVER_IP $SERVER_PORT 2>/dev/null; then
            echo "Server is ready!"
            return 0
        fi
        sleep 1
    done
    
    echo "Warning: Could not verify server is ready"
    return 1
}

# Function to run a single experiment
run_experiment() {
    local customers=$1
    local robot_type=$2
    local num_experts=$3
    local exp_name=$4
    local output_file=$5
    
    echo "Running: $customers customers, robot_type=$robot_type, experts=$num_experts"
    
    # Run client and capture output
    ./client $SERVER_IP $SERVER_PORT $customers $ORDERS_PER_CUSTOMER $robot_type 2>&1 | tee -a $output_file
    
    # Extract just the performance line (tab-separated values)
    tail -n 15 $output_file | grep -E "^[0-9]" | tail -n 1
    
    # Small delay between runs
    sleep 1
}

# ============================================================================
# EXPERIMENT 1: Regular robots, no experts
# ============================================================================
echo "===================="
echo "EXPERIMENT 1: Regular Robots (No Experts)"
echo "===================="

EXP1_OUTPUT="$OUTPUT_DIR/experiment1_regular.txt"
EXP1_DATA="$OUTPUT_DIR/experiment1_data.tsv"

echo "Starting server for Experiment 1..."
./server $SERVER_PORT 0 > $OUTPUT_DIR/server1.log 2>&1 &
SERVER_PID=$!
wait_for_server

# Header for data file
echo -e "customers\tavg_latency\tmin_latency\tmax_latency\tthroughput" > $EXP1_DATA

for customers in "${CUSTOMER_COUNTS[@]}"; do
    echo "customers=$customers" >> $EXP1_OUTPUT
    result=$(run_experiment $customers 0 0 "Exp1" $EXP1_OUTPUT)
    echo -e "$customers\t$result" >> $EXP1_DATA
done

echo "Stopping server..."
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null
sleep 2

echo ""

# ============================================================================
# EXPERIMENT 2: Special robots, 1 expert
# ============================================================================
echo "===================="
echo "EXPERIMENT 2: Special Robots (1 Expert)"
echo "===================="

EXP2_OUTPUT="$OUTPUT_DIR/experiment2_special_1expert.txt"
EXP2_DATA="$OUTPUT_DIR/experiment2_data.tsv"

echo "Starting server for Experiment 2..."
./server $SERVER_PORT 1 > $OUTPUT_DIR/server2.log 2>&1 &
SERVER_PID=$!
wait_for_server

echo -e "customers\tavg_latency\tmin_latency\tmax_latency\tthroughput" > $EXP2_DATA

for customers in "${CUSTOMER_COUNTS[@]}"; do
    echo "customers=$customers" >> $EXP2_OUTPUT
    result=$(run_experiment $customers 1 1 "Exp2" $EXP2_OUTPUT)
    echo -e "$customers\t$result" >> $EXP2_DATA
done

echo "Stopping server..."
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null
sleep 2

echo ""

# ============================================================================
# EXPERIMENT 3: Special robots, 16 experts
# ============================================================================
echo "===================="
echo "EXPERIMENT 3: Special Robots (16 Experts)"
echo "===================="

EXP3_OUTPUT="$OUTPUT_DIR/experiment3_special_16experts.txt"
EXP3_DATA="$OUTPUT_DIR/experiment3_data.tsv"

echo "Starting server for Experiment 3..."
./server $SERVER_PORT 16 > $OUTPUT_DIR/server3.log 2>&1 &
SERVER_PID=$!
wait_for_server

echo -e "customers\tavg_latency\tmin_latency\tmax_latency\tthroughput" > $EXP3_DATA

for customers in "${CUSTOMER_COUNTS[@]}"; do
    echo "customers=$customers" >> $EXP3_OUTPUT
    result=$(run_experiment $customers 1 16 "Exp3" $EXP3_OUTPUT)
    echo -e "$customers\t$result" >> $EXP3_DATA
done

echo "Stopping server..."
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null
sleep 2

echo ""

# ============================================================================
# EXPERIMENT 4: Special robots, experts = customers
# ============================================================================
echo "===================="
echo "EXPERIMENT 4: Special Robots (Experts = Customers)"
echo "===================="

EXP4_OUTPUT="$OUTPUT_DIR/experiment4_special_matching.txt"
EXP4_DATA="$OUTPUT_DIR/experiment4_data.tsv"

echo -e "customers\tavg_latency\tmin_latency\tmax_latency\tthroughput" > $EXP4_DATA

for customers in "${CUSTOMER_COUNTS[@]}"; do
    echo "Starting server with $customers experts..."
    ./server $SERVER_PORT $customers > $OUTPUT_DIR/server4_$customers.log 2>&1 &
    SERVER_PID=$!
    wait_for_server
    
    echo "customers=$customers" >> $EXP4_OUTPUT
    result=$(run_experiment $customers 1 $customers "Exp4" $EXP4_OUTPUT)
    echo -e "$customers\t$result" >> $EXP4_DATA
    
    echo "Stopping server..."
    kill $SERVER_PID 2>/dev/null
    wait $SERVER_PID 2>/dev/null
    sleep 2
done

echo ""
echo "===================="
echo "All experiments complete!"
echo "Results saved to: $OUTPUT_DIR/"
echo "===================="
echo ""
echo "Data files:"
echo "  - $EXP1_DATA"
echo "  - $EXP2_DATA"
echo "  - $EXP3_DATA"
echo "  - $EXP4_DATA"
echo ""
echo "You can now plot these results using plot_results.py"

# Make script executable
# chmod +x run_experiments.sh