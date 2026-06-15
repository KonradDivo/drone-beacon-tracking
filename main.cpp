#include "BeaconTracker.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "=== SYSTÈME D'ASSERVISSEMENT VISUEL DU DRONE ===" << std::endl;

    std::string videoPath = "";
    if (argc < 2) {
        std::cout << "[USAGE] Spécifiez la vidéo du vol : ./drone_beacon_tracking.exe video.mp4" << std::endl;
        return -1;
        //videoPath = "";
    }
    else{
        videoPath = argv[1];
    }
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Erreur : Flux vidéo introuvable." << std::endl;
        return -1;
    }

    BeaconTracker tracker;
    cv::Mat frame;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Traiter l'image avec notre classe encapsulée
        Point2D beacon = tracker.processFrame(frame);

        // Envoyer les coordonnées de guidage à la console (simulant l'envoi aux moteurs)
        if (beacon.x != -1) {
            std::cout << "[GUIDAGE] Balise détectée ! Centrage horizontal X: " << beacon.x 
                      << " | Y: " << beacon.y << std::endl;
        } else {
            std::cout << "[CRITIQUE] Alerte : Balise hors de vue !" << std::endl;
        }

        // Afficher la vue caméra du drone
        cv::imshow("Caméra Drone - Suivi de Balise", frame);

        if (cv::waitKey(20) == 27) break; // Échap pour couper
    }

    return 0;
}
