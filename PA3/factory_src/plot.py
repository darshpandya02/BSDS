import matplotlib.pyplot as plt
import numpy as np

# =============================================================================
# DATA INPUT SECTION - FILL IN YOUR DATA HERE
# =============================================================================

# Experiment 1 Data
exp1_servers = [1, 2, 3]  # Number of servers
exp1_mean_latency = [136.207]  # Mean latency in ms
exp1_min_latency = [ 18.624 ]    # Min latency in ms for 1 server   
exp1_max_latency = [102292.157]   # Max latency in ms for 1 server
exp1_throughput = [ 55675.167]    # Throughput (requests/sec or your unit) for 1 server

# Experiment 2 Data
exp2_servers = [1, 2, 3]  # Number of servers
exp2_mean_latency = []  # Mean latency in ms for 2 servers
exp2_min_latency = []    # Min latency in ms for 2 servers
exp2_max_latency = []
exp2_throughput = []

# =============================================================================
# PLOTTING FUNCTIONS
# =============================================================================

def plot_latency(customers, mean_lat, min_lat, max_lat, exp_num):
    """Plot latency graph with mean, min, and max values"""
    plt.figure(figsize=(10, 6))
    
    # Create x-axis positions with equal spacing
    x_pos = np.arange(len(customers))
    
    plt.plot(x_pos, mean_lat, marker='o', linewidth=2, markersize=8, label='Mean Latency')
    plt.plot(x_pos, min_lat, marker='s', linewidth=2, markersize=8, label='Min Latency')
    plt.plot(x_pos, max_lat, marker='^', linewidth=2, markersize=8, label='Max Latency')
    
    # Set logarithmic y-axis (base 10)
    plt.yscale('log', base=10)

    plt.xlabel('Number of Customers', fontsize=12)
    plt.ylabel('Latency (microseconds)', fontsize=12)
    plt.title(f'Experiment {exp_num}: Latency vs Number of Customers', fontsize=14, fontweight='bold')
    plt.xticks(x_pos, customers)  # Set custom x-tick labels
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    plt.savefig(f'experiment_{exp_num}_latency.png', dpi=300, bbox_inches='tight')
    plt.show()

def plot_throughput(customers, throughput, exp_num):
    """Plot throughput graph"""
    plt.figure(figsize=(10, 6))
    
    # Create x-axis positions with equal spacing
    x_pos = np.arange(len(customers))
    
    plt.plot(x_pos, throughput, marker='o', linewidth=2, markersize=8, color='green')
    
    plt.xlabel('Number of Customers', fontsize=12)
    plt.ylabel('Throughput (requests/sec)', fontsize=12)
    plt.title(f'Experiment {exp_num}: Throughput vs Number of Customers', fontsize=14, fontweight='bold')
    plt.xticks(x_pos, customers)  # Set custom x-tick labels
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    plt.savefig(f'experiment_{exp_num}_throughput.png', dpi=300, bbox_inches='tight')
    plt.show()

# =============================================================================
# GENERATE ALL PLOTS
# =============================================================================

print("Generating plots for all experiments...")

# Experiment 1
print("\nWrite Performance graphs...")
plot_latency(exp1_customers, exp1_mean_latency, exp1_min_latency, exp1_max_latency, 1)
plot_throughput(exp1_customers, exp1_throughput, 1)

# Experiment 2
print("\Read Performance graphs...")
plot_latency(exp2_customers, exp2_mean_latency, exp2_min_latency, exp2_max_latency, 2)
plot_throughput(exp2_customers, exp2_throughput, 2)

