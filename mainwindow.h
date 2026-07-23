#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QStatusBar>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include "TCHBYCHV.h"

class UpdateChecker;
class LangueManager;
class Project;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onCompute();
    void onExportText();
    void onClearPoints();
    void onInputModeChanged(int index);
    void onZoomIn();
    void onZoomOut();
    void onResetView();
    void showAbout();
    void checkUpdate();
    void loadProject();
    void saveProject();
    void changerLangue(const QString& code);

private:
    void buildUI();
    void createMenus();
    void retranslateUi();
    void computeChebyshev();
    void updateChart();
    void autoRange();
    void saveSettings();
    void loadSettings();
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

    // Feature: Menu actions
    QMenu *m_menuFichier, *m_menuEdition, *m_menuAffichage;
    QMenu *m_menuLangue, *m_menuAide;
    QAction *m_aAbout, *m_aCheckUpdate;
    QAction *m_aLoadProject, *m_aSaveProject, *m_aExport;
    QAction *m_aQuit, *m_aClear, *m_aZoomIn, *m_aZoomOut, *m_aAutoRange;

    // Feature: Update checker
    UpdateChecker *m_updateChecker;

    // Feature: Language
    LangueManager *m_langue;

    // Feature: Project
    Project *m_project;
};