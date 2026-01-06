#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <iostream>

int main() {
    cv::Mat image(cv::Mat::zeros(200, 200, CV_8UC3));
    cv::circle(image, {100, 100}, 30, {0, 0, 255}, -1);

    using namespace ftxui;
    // Create a simple document with three text elements.
    Element document = hbox({
        text("left")   | border,
        text("middle") | border | flex,
        text("right")  | border,
    });
    
    // Create a screen with full width and height fitting the document.
    auto screen = Screen::Create(
        Dimension::Full(),       // Width
        Dimension::Fit(document) // Height
    );
    
    // Render the document onto the screen.
    Render(screen, document);
    
    // Print the screen to the console.
    screen.Print();
}