# Non-Invasive Glucose Monitoring System (Research-Based)
<p align="center">
  <img src="" alt="Prototype " width="48%" />![Uploading Picture1.png…]()

</p>
This repository hosts the initial firmware, data analysis pipeline, and research documentation for a non-invasive blood glucose trend estimation system using PPG sensors.

##  Technical Highlights
- **Sensor Customization:** Repurposed MAX30102/MAX30105 sensor to extract glucose readings using IR/Red light and SpO2 levels.
- **Machine Learning Pipeline:** Trained and validated a Random Forest regressor on Google Colab using Python, incorporating both Kaggle datasets and custom clinical trials.
- **Clinical Validation:** Evaluated model accuracy using rigorous metrics: **RMSE, R², MAE, MARD, and Clarke Error Grid analysis**.
- **Edge Deployment:** Generated optimized C-code for ESP32 deployment.
- **Data Automation:** Developed a pipeline for raw sensor data logging to Excel.

## 📂 Repository Contents
* **`Firmware/`**: ESP32 C3 code for sensor integration and signal filtering.
* **`Data-Analysis/`**: Colab notebook demonstrating the Random Forest pipeline.
* **`Documentation/`**: Initial draft of the research paper (OI-JET).
* **`Results/`**: Final performance metrics, graphs, and clinical data analysis.

##  Development Status
This repository is in **"Early Release"**. 
* **Note:** The cloud automation pipeline (n8n/Supabase) is currently undergoing refactoring to improve stability and is temporarily removed from the firmware. It will be re-integrated in future updates.

*Technical details and exhaustive experimental data are ongoing and subject to frequent updates.*
