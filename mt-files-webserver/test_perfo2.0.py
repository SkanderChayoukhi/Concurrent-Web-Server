import time
import threading
import requests
import matplotlib.pyplot as plt
import argparse
from concurrent.futures import ThreadPoolExecutor

# Function to send a request to the server
def send_request(url):
    try:
        response = requests.get(url)
        return response.status_code
    except Exception as e:
        print(f"Error: {e}")
        return None

# Function to run concurrent requests using ThreadPoolExecutor for better thread management
def test_server(url, num_requests, num_threads):
    with ThreadPoolExecutor(max_workers=num_threads) as executor:
        start_time = time.time()
        # Submitting all requests to the executor
        futures = [executor.submit(send_request, url) for _ in range(num_requests)]
        # Waiting for all futures to complete
        results = [future.result() for future in futures]
        end_time = time.time()
        elapsed_time = end_time - start_time
    return elapsed_time

# Function to perform performance tests
def performance_test(server_url, thread_pools, request_counts):
    results = {}

    for threads in thread_pools:
        print(f"\nTesting with {threads} threads...")
        results[threads] = []

        for req_count in request_counts:
            print(f" - Sending {req_count} simultaneous requests...")
            elapsed_time = test_server(server_url, req_count, threads)
            print(f"   -> Time taken: {elapsed_time:.2f} seconds")
            results[threads].append(elapsed_time)

    return results

# Plot results with more precise details
def plot_results(results, request_counts):
    plt.figure(figsize=(10, 6))
    for threads, times in results.items():
        plt.plot(request_counts, times, marker='o', label=f"{threads} threads")

    plt.xlabel("Number of Simultaneous Requests")
    plt.ylabel("Time Taken (seconds)")
    plt.title("Server Performance: Time vs Number of Requests")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

# Main function
def main():
    parser = argparse.ArgumentParser(description="Server Performance Tester")
    parser.add_argument("-u", "--url", type=str, required=True, help="Server URL (e.g., http://localhost:10000/file)")
    args = parser.parse_args()

    server_url = args.url
    # thread_pools = [2, 4, 8, 16]  # Different thread pool sizes to test
    # request_counts = [10, 50, 100, 200, 500]  # Number of simultaneous requests

    thread_pools = [16, 32,64,128]
    request_counts = [10, 50, 100, 200, 500 , 1000]
  
    # thread_pools = [64,128, 256, 512]
    # request_counts = [ 1000,2000, 4000, 5000, 10000] 

    print(f"Testing server: {server_url}")
    results = performance_test(server_url, thread_pools, request_counts)
    plot_results(results, request_counts)

if __name__ == "__main__":
    main()
