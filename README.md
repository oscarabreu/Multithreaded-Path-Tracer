# Ray Tracing with Multithreading

![377687585_6643121792468763_8300831540565573253_n](https://github.com/oscarabreu/Multithreaded-Path-Tracer/assets/99779654/08b1c2b7-66d7-42bb-ac00-272a8c584245)


This project is an enhanced implementation of Peter Shirley's _Ray Tracing in One Weekend_. The core ray tracing logic remains the same, but the program now benefits from multithreading, extensive commenting, and improved performance.

## Table of Contents
- [Features](#features)
- [Performance](#performance)
- [Original Work Credit](#original-work-credit)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Features
- **Multithreading**: Leveraging the power of concurrent processing, the rendering process is now distributed across multiple threads, significantly improving performance.
  
- **Progress Display**: Real-time feedback on rendering progress, displayed in terms of blocks completed.
  
- **Optimized Output Buffering**: Instead of writing pixel data directly to the standard output, the program stores it in a buffer and then outputs all at once, further enhancing efficiency.

- **Comprehensive Commenting**: The code is extensively commented for better readability and understanding.

## Performance
The addition of multithreading has significantly reduced the rendering time. The program now utilizes the `std::thread` library to spawn multiple threads based on the hardware's capability. This parallel processing is complemented by an optimized output buffering mechanism, ensuring a smoother user experience.

## Original Work Credit
All foundational concepts and the base ray tracing logic are credited to **Peter Shirley** and his book [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html). This project has been built upon that foundation with additional features and optimizations.

## Usage
1. Compile the project using a suitable C++ compiler supporting C++11 or higher.
2. Run the executable.
3. Observe the ray-traced scene and the performance metrics.

## Contributing
Feel free to fork and make improvements. If you come up with significant performance enhancements or additional features, please consider submitting a pull request.

## License
This project is open-source. While the enhancements and optimizations are original, credit for the base ray tracing logic goes to Peter Shirley. If you use or share this implementation, please acknowledge both the original work and the enhancements made here.
  
