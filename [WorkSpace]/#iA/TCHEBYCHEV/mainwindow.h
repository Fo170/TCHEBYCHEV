#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include "TCHBYCHV.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCompute();
    void onExportText();
    void onClearPoints();
    void onInputModeChanged(int index);
    void onZoomIn();
    void onZoomOut();
    void onResetView();

private:
    void buildUI();
    void computeChebyshev();
    void updateChart();
    void autoRange();
    Ldbl functionFT(int p, Ldbl t1);

    // Input
    QComboBox      *m_inputMode;
    QLineEdit      *m_nbPoints;
    QLineEdit      *m_xmin, *m_xmax;
    QLineEdit      *m_functionExpr;
    QTableWidget   *m_pointsTable;
    QPushButton    *m_btnCompute, *m_btnClear, *m_btnExport;
    QPushButton    *m_btnLoadFile;

    // Output
    QTextEdit      *m_output;
    QLabel         *m_errLabel;
    QChartView     *m_chartView;
    QChart         *m_chart;

    // Data
    QVector<QPointF> m_inputPoints;
    QVector<QPointF> m_fittedPoints;
    QVector<Ldbl>    m_coeffs;

    // Chart series
    QScatterSeries *m_inputSeries;
    QLineSeries    *m_fittedSeries;

    int m_np;
    Ldbl m_xminVal, m_xmaxVal;
};
