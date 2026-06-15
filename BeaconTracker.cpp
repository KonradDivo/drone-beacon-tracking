#include "BeaconTracker.hpp"

BeaconTracker::BeaconTracker() {
    // Configuration pour détecter une balise de couleur vive (Ex: Orange/Rouge fluo)
    m_lowerHSV = cv::Scalar(0, 120, 70);
    m_upperHSV = cv::Scalar(10, 255, 255);
    m_minArea = 400; // Un contour de moins de 400 pixels carrés est ignoré
}

Point2D BeaconTracker::processFrame(cv::Mat& frame) {
    Point2D targetCenter;
    cv::Mat hsvFrame, mask;

    // 1. Convertir l'image du drone (BGR) vers l'espace de couleur HSV (plus stable)
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

    // 2. Seuil de couleur : tout ce qui est dans l'intervalle devient blanc, le reste noir
    cv::inRange(hsvFrame, m_lowerHSV, m_upperHSV, mask);

    // 3. Nettoyer l'image (supprimer les petits bruits blancs parasites)
    cv::erode(mask, mask, cv::Mat());
    cv::dilate(mask, mask, cv::Mat());

    // 4. Détecter les contours géométriques des formes blanches
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    double maxArea = 0;
    int largestContourIdx = -1;

    // Chercher la forme la plus grande (la balise principale)
    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > m_minArea && area > maxArea) {
            maxArea = area;
            largestContourIdx = static_cast<int>(i);
        }
    }

    // 5. Si la balise est trouvée, calculer son centre de gravité mathématique
    if (largestContourIdx != -1) {
        cv::Moments mu = cv::moments(contours[largestContourIdx]);
        
        // Formule mathématique du centre de masse
        targetCenter.x = static_cast<int>(mu.m10 / mu.m00);
        targetCenter.y = static_cast<int>(mu.m01 / mu.m00);

        // Dessiner le contour de la balise en bleu sur l'image
        cv::drawContours(frame, contours, largestContourIdx, cv::Scalar(255, 0, 0), 2);
        
        // Dessiner un point rouge au centre exact
        cv::circle(frame, cv::Point(targetCenter.x, targetCenter.y), 5, cv::Scalar(0, 0, 255), -1);
    }

    return targetCenter; // Si -1, le drone sait qu'il a perdu la balise de vue
}
