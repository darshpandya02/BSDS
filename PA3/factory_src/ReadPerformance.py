#!/usr/bin/env python3
"""
Read Performance Test - Multi-Factory Robot System
Measures read latency and throughput with varying number of servers
"""

import matplotlib.pyplot as plt
import numpy as np


def plot_read_performance(num_servers, latencies, throughputs, save_file=None):
    """Plot read performance graphs"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
    
    # Graph 1: Read Latency vs Number of Servers
    ax1.plot(num_servers, latencies, 'o-', linewidth=2, markersize=8, 
             color='#06A77D', label='Read Latency')
    ax1.set_xlabel('Number of Servers', fontsize=12, fontweight='bold')
    ax1.set_ylabel('Average Read Latency (μs)', fontsize=12, fontweight='bold')
    ax1.set_title('Read Latency vs Number of Servers\n(128 customers, record reads)', 
                  fontsize=13, fontweight='bold')
    ax1.grid(True, alpha=0.3, linestyle='--')
    ax1.set_xticks(num_servers)
    ax1.legend(loc='upper left', fontsize=10)
    
    # Add value labels on points
    for i, (x, y) in enumerate(zip(num_servers, latencies)):
        ax1.annotate(f'{y:.1f}', xy=(x, y), xytext=(0, 10), 
                    textcoords='offset points', ha='center', fontsize=10)
    
    # Graph 2: Read Throughput vs Number of Servers
    ax2.plot(num_servers, throughputs, 's-', linewidth=2, markersize=8, 
             color='#D81159', label='Read Throughput')
    ax2.set_xlabel('Number of Servers', fontsize=12, fontweight='bold')
    ax2.set_ylabel('Read Throughput (ops/sec)', fontsize=12, fontweight='bold')
    ax2.set_title('Read Throughput vs Number of Servers\n(128 customers, record reads)', 
                  fontsize=13, fontweight='bold')
    ax2.grid(True, alpha=0.3, linestyle='--')
    ax2.set_xticks(num_servers)
    ax2.legend(loc='upper right', fontsize=10)
    
    # Add value labels on points
    for i, (x, y) in enumerate(zip(num_servers, throughputs)):
        ax2.annotate(f'{y:.1f}', xy=(x, y), xytext=(0, -15), 
                    textcoords='offset points', ha='center', fontsize=10)
    
    plt.tight_layout()
    
    if save_file:
        plt.savefig(save_file, dpi=300, bbox_inches='tight')
        print(f"\nGraph saved to: {save_file}")
    
    plt.show()


def main():
    print("\n" + "="*60)
    print("READ PERFORMANCE MEASUREMENT SCRIPT")
    print("="*60)
    
    num_servers = [1, 1, 2, 2]
    latencies = [544.241, 1766.172, 788.086, 1864.684]
    throughputs = [71836.447, 81901.079, 77726.937, 82606.474]
    
    if num_servers is None:
        print("\nData collection failed. Exiting.")
        return
    
    print("\n" + "="*60)
    print("DATA SUMMARY")
    print("="*60)
    print(f"{'Servers':<10} {'Latency (μs)':<20} {'Throughput (ops/s)':<20}")
    print("-" * 60)
    for n, lat, thr in zip(num_servers, latencies, throughputs):
        print(f"{n:<10} {lat:<20.3f} {thr:<20.3f}")
    print("="*60)
    
    save = input("\nSave graph to file? (y/n): ").lower().strip()
    save_file = "read_performance.png" if save == 'y' else None
    
    plot_read_performance(num_servers, latencies, throughputs, save_file)
    
    print("\nAnalysis:")
    lat_change = ((latencies[2]-latencies[0])/latencies[0]*100) if latencies[0] != 0 else 0
    thr_change = ((throughputs[2]-throughputs[0])/throughputs[0]*100) if throughputs[0] != 0 else 0
    print(f"- Latency change from 1 to 3 servers: {lat_change:+.1f}%")
    print(f"- Throughput change from 1 to 3 servers: {thr_change:+.1f}%")
    print("\nExpected Behavior:")
    print("- Read latency should remain relatively constant (no replication overhead)")
    print("- May see slight increase with 128 concurrent threads due to mutex contention")


if __name__ == "__main__":
    main()