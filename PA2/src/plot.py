import matplotlib.pyplot as plt
import numpy as np

# =============================================================================
# DATA INPUT SECTION - FILL IN YOUR DATA HERE
# =============================================================================

# Experiment 1 Data
exp1_customers = [10, 20, 30, 40, 50]  # Number of customers
exp1_mean_latency = [100, 150, 200, 250, 300]  # Mean latency in ms
exp1_min_latency = [80, 120, 160, 200, 240]    # Min latency in ms
exp1_max_latency = [120, 180, 240, 300, 360]   # Max latency in ms
exp1_throughput = [100, 180, 240, 280, 300]    # Throughput (requests/sec or your unit)

# Experiment 2 Data
exp2_customers = [10, 20, 30, 40, 50]
exp2_mean_latency = [90, 140, 190, 240, 290]
exp2_min_latency = [70, 110, 150, 190, 230]
exp2_max_latency = [110, 170, 230, 290, 350]
exp2_throughput = [110, 190, 250, 290, 310]

# Experiment 3 Data
exp3_customers = [10, 20, 30, 40, 50]
exp3_mean_latency = [95, 145, 195, 245, 295]
exp3_min_latency = [75, 115, 155, 195, 235]
exp3_max_latency = [115, 175, 235, 295, 355]
exp3_throughput = [105, 185, 245, 285, 305]

# Experiment 4 Data
exp4_customers = [10, 20, 30, 40, 50]
exp4_mean_latency = [105, 155, 205, 255, 305]
exp4_min_latency = [85, 125, 165, 205, 245]
exp4_max_latency = [125, 185, 245, 305, 365]
exp4_throughput = [95, 175, 235, 275, 295]

# =============================================================================
# PLOTTING FUNCTIONS
# =============================================================================

def plot_latency(customers, mean_lat, min_lat, max_lat, exp_num):
    """Plot latency graph with mean, min, and max values"""
    plt.figure(figsize=(10, 6))
    
    plt.plot(customers, mean_lat, marker='o', linewidth=2, markersize=8, label='Mean Latency')
    plt.plot(customers, min_lat, marker='s', linewidth=2, markersize=8, label='Min Latency')
    plt.plot(customers, max_lat, marker='^', linewidth=2, markersize=8, label='Max Latency')
    
    plt.xlabel('Number of Customers', fontsize=12)
    plt.ylabel('Latency (ms)', fontsize=12)
    plt.title(f'Experiment {exp_num}: Latency vs Number of Customers', fontsize=14, fontweight='bold')
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    plt.savefig(f'experiment_{exp_num}_latency.png', dpi=300, bbox_inches='tight')
    plt.show()

def plot_throughput(customers, throughput, exp_num):
    """Plot throughput graph"""
    plt.figure(figsize=(10, 6))
    
    plt.plot(customers, throughput, marker='o', linewidth=2, markersize=8, color='green')
    
    plt.xlabel('Number of Customers', fontsize=12)
    plt.ylabel('Throughput (requests/sec)', fontsize=12)
    plt.title(f'Experiment {exp_num}: Throughput vs Number of Customers', fontsize=14, fontweight='bold')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    plt.savefig(f'experiment_{exp_num}_throughput.png', dpi=300, bbox_inches='tight')
    plt.show()

# =============================================================================
# GENERATE ALL PLOTS
# =============================================================================

print("Generating plots for all experiments...")

# Experiment 1
print("\nGenerating Experiment 1 graphs...")
plot_latency(exp1_customers, exp1_mean_latency, exp1_min_latency, exp1_max_latency, 1)
plot_throughput(exp1_customers, exp1_throughput, 1)

# Experiment 2
print("\nGenerating Experiment 2 graphs...")
plot_latency(exp2_customers, exp2_mean_latency, exp2_min_latency, exp2_max_latency, 2)
plot_throughput(exp2_customers, exp2_throughput, 2)

# Experiment 3
print("\nGenerating Experiment 3 graphs...")
plot_latency(exp3_customers, exp3_mean_latency, exp3_min_latency, exp3_max_latency, 3)
plot_throughput(exp3_customers, exp3_throughput, 3)

# Experiment 4
print("\nGenerating Experiment 4 graphs...")
plot_latency(exp4_customers, exp4_mean_latency, exp4_min_latency, exp4_max_latency, 4)
plot_throughput(exp4_customers, exp4_throughput, 4)

print("\nAll plots generated successfully!")
print("Graphs saved as PNG files in the current directory.")