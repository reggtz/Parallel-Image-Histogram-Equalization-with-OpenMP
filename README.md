# Parallel Image Histogram Equalization with OpenMP

## Overview

This project implements histogram equalization for grayscale images using both sequential and parallel programming techniques in C. The main objective is to improve image contrast while evaluating the performance benefits of parallel computing with OpenMP.

The application processes an input image, generates its histogram, applies histogram equalization, exports the resulting image, and compares the sequential and parallel implementations using performance metrics such as Speedup, Efficiency, and Overhead.

---

## Features

- Histogram equalization for grayscale images
- Sequential implementation
- Parallel implementation using OpenMP
- Automatic histogram generation
- CSV export of histogram data
- Performance benchmarking
- Speedup calculation
- Efficiency calculation
- Overhead calculation

---

## Technologies

- C
- OpenMP
- stb_image
- stb_image_write
- CSV

---

## Project Workflow

1. Load an input image (.jpg).
2. Compute the original histogram.
3. Calculate the cumulative distribution function (CDF).
4. Apply histogram equalization.
5. Generate the equalized image.
6. Export histogram data to CSV.
7. Measure execution time.
8. Compare sequential and parallel performance.

---

## Parallelization Strategy

The computationally expensive sections of the algorithm were parallelized using OpenMP while preserving correctness.

The implementation makes use of:

- `#pragma omp parallel`
- `#pragma omp for`
- `reduction`
- `atomic`
- `barrier`
- `single`

The cumulative distribution function (CDF) remains sequential because it contains data dependencies that would produce incorrect results if executed in parallel.

---

## Performance Metrics

The program automatically measures:

- Sequential execution time
- Parallel execution time
- Speedup
- Efficiency
- Overhead
- Number of threads used

These metrics allow an objective comparison between both implementations.

---

## Repository Structure

```
.
├── src/
├── images/
├── output/
├── csv/
├── README.md
└── LICENSE
```

---

## Example Output

The program generates:

- Equalized image (Sequential)
- Equalized image (Parallel)
- Original histogram
- Equalized histogram
- CSV files containing histogram values
- Performance metrics displayed in the console

---

## Skills Demonstrated

- Parallel Computing
- Image Processing
- Performance Optimization
- Algorithm Analysis
- Concurrent Programming
- OpenMP
- C Programming
- Memory Management
- Performance Benchmarking

---

## Future Improvements

- RGB histogram equalization
- CUDA implementation for GPU acceleration
- SIMD optimization
- Batch image processing
- Support for multiple image formats
- Interactive visualization of performance metrics

---

## Learning Outcomes

This project provided practical experience in parallel programming by comparing sequential and OpenMP-based implementations of the same algorithm. It also reinforced concepts related to synchronization, race conditions, workload distribution, and performance analysis.

The implementation demonstrates that parallel computing can significantly reduce execution time for computationally intensive image processing tasks while maintaining the correctness of the output.

---

## Author

Frida Regina Gutierrez Benavides

Faculty of Engineering

National Autonomous University of Mexico (UNAM)
