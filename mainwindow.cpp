#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_np(0), m_xminVal(0), m_xmaxVal(0)
{
    setWindowTitle("TCHEBYCHEV - Approximation de fonctions");
    resize(1200, 800);
    buildUI();
}

MainWindow::~MainWindow() {}

void MainWindow::buildUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // ---- Left panel: Input ----
    QVBoxLayout *leftPanel = new QVBoxLayout();

    QGroupBox *inputGroup = new QGroupBox("Saisie");
    QVBoxLayout *inputLayout = new QVBoxLayout(inputGroup);

    m_inputMode = new QComboBox();
    m_inputMode->addItem("Points (saisie manuelle)");
    m_inputMode->addItem("Points (fichier texte)");
    m_inputMode->addItem("Fonction mathematique");
    connect(m_inputMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onInputModeChanged);

    QFormLayout *paramLayout = new QFormLayout();
    m_nbPoints  = new QLineEdit("18");
    m_xmin      = new QLineEdit("-3.14159");
    m_xmax      = new QLineEdit("3.14159");
    m_functionExpr = new QLineEdit("4*sin(x)/(exp(x)+exp(-x))");
    m_functionExpr->setEnabled(false);
    paramLayout->addRow("Nb points:", m_nbPoints);
    paramLayout->addRow("X min:", m_xmin);
    paramLayout->addRow("X max:", m_xmax);
    paramLayout->addRow("Fonction f(x):", m_functionExpr);

    m_pointsTable = new QTableWidget(0, 2);
    m_pointsTable->setHorizontalHeaderLabels({"X", "Y"});
    m_pointsTable->horizontalHeader()->setStretchLastSection(true);

    m_btnLoadFile = new QPushButton("Charger fichier...");
    m_btnLoadFile->setEnabled(false);
    connect(m_btnLoadFile, &QPushButton::clicked, this, [this](){
        QString path = QFileDialog::getOpenFileName(this, "Charger points", "", "Textes (*.txt *.csv *.dat)");
        if (path.isEmpty()) return;
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly)) return;
        QTextStream in(&f);
        m_pointsTable->setRowCount(0);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith('#')) continue;
            QStringList parts = line.split(QRegularExpression("[\\s,;]+"));
            if (parts.size() >= 2) {
                int r = m_pointsTable->rowCount();
                m_pointsTable->insertRow(r);
                m_pointsTable->setItem(r, 0, new QTableWidgetItem(parts[0]));
                m_pointsTable->setItem(r, 1, new QTableWidgetItem(parts[1]));
            }
        }
        f.close();
    });

    QHBoxLayout *btnRow = new QHBoxLayout();
    m_btnCompute = new QPushButton("Calculer");
    m_btnClear   = new QPushButton("Effacer");
    m_btnExport  = new QPushButton("Exporter");
    connect(m_btnCompute, &QPushButton::clicked, this, &MainWindow::onCompute);
    connect(m_btnClear,   &QPushButton::clicked, this, &MainWindow::onClearPoints);
    connect(m_btnExport,  &QPushButton::clicked, this, &MainWindow::onExportText);
    btnRow->addWidget(m_btnCompute);
    btnRow->addWidget(m_btnClear);
    btnRow->addWidget(m_btnExport);

    inputLayout->addWidget(m_inputMode);
    inputLayout->addLayout(paramLayout);
    inputLayout->addWidget(m_btnLoadFile);
    inputLayout->addWidget(m_pointsTable);
    inputLayout->addLayout(btnRow);

    leftPanel->addWidget(inputGroup);

    // Output text
    QGroupBox *outGroup = new QGroupBox("Resultats");
    QVBoxLayout *outLayout = new QVBoxLayout(outGroup);
    m_output = new QTextEdit();
    m_output->setReadOnly(true);
    m_output->setFont(QFont("Courier New", 9));
    m_errLabel = new QLabel("Erreur Absolue Max: --");
    outLayout->addWidget(m_errLabel);
    outLayout->addWidget(m_output);
    leftPanel->addWidget(outGroup);

    mainLayout->addLayout(leftPanel, 2);

    // ---- Right panel: Chart ----
    QVBoxLayout *rightPanel = new QVBoxLayout();

    m_chart = new QChart();
    m_chart->setTitle("Approximation de Tchebychev");
    m_chart->setAnimationOptions(QChart::SeriesAnimations);

    m_inputSeries = new QScatterSeries();
    m_inputSeries->setName("Points saisis");
    m_inputSeries->setMarkerSize(8);
    m_inputSeries->setColor(Qt::red);

    m_fittedSeries = new QLineSeries();
    m_fittedSeries->setName("Approximation");
    m_fittedSeries->setColor(Qt::blue);

    m_chart->addSeries(m_inputSeries);
    m_chart->addSeries(m_fittedSeries);

    m_chart->createDefaultAxes();

    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *chartBtnRow = new QHBoxLayout();
    QPushButton *zoomIn  = new QPushButton("Zoom +");
    QPushButton *zoomOut = new QPushButton("Zoom -");
    QPushButton *resetV  = new QPushButton("Auto-range");
    connect(zoomIn,  &QPushButton::clicked, this, &MainWindow::onZoomIn);
    connect(zoomOut, &QPushButton::clicked, this, &MainWindow::onZoomOut);
    connect(resetV,  &QPushButton::clicked, this, &MainWindow::onResetView);
    chartBtnRow->addWidget(zoomIn);
    chartBtnRow->addWidget(zoomOut);
    chartBtnRow->addWidget(resetV);
    chartBtnRow->addStretch();

    rightPanel->addLayout(chartBtnRow);
    rightPanel->addWidget(m_chartView);

    mainLayout->addLayout(rightPanel, 3);
}

void MainWindow::onInputModeChanged(int index)
{
    bool isFunction = (index == 2);
    m_functionExpr->setEnabled(isFunction);
    m_pointsTable->setEnabled(!isFunction);
    m_btnLoadFile->setEnabled(index == 1);
}

void MainWindow::onClearPoints()
{
    m_pointsTable->setRowCount(0);
    m_inputPoints.clear();
    m_fittedPoints.clear();
    m_coeffs.clear();
    m_output->clear();
    m_errLabel->setText("Erreur Absolue Max: --");
    m_inputSeries->clear();
    m_fittedSeries->clear();
}

void MainWindow::onCompute()
{
    computeChebyshev();
    updateChart();
}

void MainWindow::computeChebyshev()
{
    m_output->clear();
    m_inputPoints.clear();
    m_fittedPoints.clear();
    m_coeffs.clear();

    m_np = m_nbPoints->text().toInt();
    if (m_np < 2 || m_np > 255) {
        m_output->setTextColor(Qt::red);
        m_output->append("Erreur: nb points entre 2 et 255");
        return;
    }

    m_xminVal = m_xmin->text().toDouble();
    m_xmaxVal = m_xmax->text().toDouble();

    int mode = m_inputMode->currentIndex();

    // Collect input points
    if (mode == 0 || mode == 1) {
        // Manual or file input
        for (int r = 0; r < m_pointsTable->rowCount(); r++) {
            QTableWidgetItem *xi = m_pointsTable->item(r, 0);
            QTableWidgetItem *yi = m_pointsTable->item(r, 1);
            if (xi && yi) {
                Ldbl x = xi->text().toDouble();
                Ldbl y = yi->text().toDouble();
                m_inputPoints.append(QPointF((double)x, (double)y));
            }
        }
        m_np = m_inputPoints.size();
        if (m_np < 2) {
            m_output->setTextColor(Qt::red);
            m_output->append("Erreur: pas assez de points");
            return;
        }
    }

    // Build TCHBYCHV and compute coefficients
    TCHBYCHV tch(m_np);
    tch.init(m_xminVal, m_xmaxVal);

    // Compute coefficients: Ak = 2/N * Sum[j=0..N-1]{ yj * Tk(vj) }
    Ldbl dPI = PI_2 / m_np;
    m_coeffs.resize(m_np);
    for (int i = 0; i < m_np; i++)
        m_coeffs[i] = 0.0L;

    for (int j = 0; j < m_np; j++) {
        Ldbl vj = cosl((2 * j + 1) * dPI);
        Ldbl xj = m_xminVal + (1.0L + vj) * (m_xmaxVal - m_xminVal) / 2.0L;
        Ldbl yj;

        if (mode == 2) {
            // Function evaluation
            yj = functionFT(j, xj);
            m_inputPoints.append(QPointF((double)xj, (double)yj));
        } else {
            // Interpolate input points to Chebyshev nodes
            // Find nearest input point
            int nearest = 0;
            Ldbl minDist = 1e100;
            for (int k = 0; k < m_inputPoints.size(); k++) {
                Ldbl d = fabsl(xj - (Ldbl)m_inputPoints[k].x());
                if (d < minDist) { minDist = d; nearest = k; }
            }
            yj = m_inputPoints[nearest].y();
        }

        tch.Y[j] = yj;
        tch.X[j] = xj;

        m_coeffs[0] += yj / 2.0L;
        m_coeffs[1] += yj * vj;

        Ldbl Tk_ = 1.0L, Tk = vj;
        for (int k = 2; k < m_np; k++) {
            Ldbl Ti = 2.0L * vj * Tk - Tk_;
            Tk_ = Tk;
            Tk  = Ti;
            m_coeffs[k] += yj * Tk;
        }
    }

    Ldbl Nfactor = 2.0L / m_np;
    for (int i = 0; i < m_np; i++)
        m_coeffs[i] *= Nfactor;

    // Coefficients output
    QString outTxt;
    outTxt += "Coefficients A(k):\n";
    outTxt += "-----------------\n";
    for (int i = 0; i < m_np; i++)
        outTxt += QString("A(%1) = %2\n").arg(i).arg((double)m_coeffs[i]);

    // Evaluate fit and compute error
    Ldbl errAbsMax = 0.0L;
    outTxt += "\nVerification:\n";
    outTxt += "-------------\n";

    for (int j = 0; j < m_np; j++) {
        Ldbl vj = cosl((2 * j + 1) * dPI);
        Ldbl xj = m_xminVal + (1.0L + vj) * (m_xmaxVal - m_xminVal) / 2.0L;

        Ldbl yOrig = tch.Y[j];
        Ldbl yCalc = m_coeffs[0] + m_coeffs[1] * vj;

        Ldbl Tk_ = 1.0L, Tk = vj;
        for (int k = 2; k < m_np; k++) {
            Ldbl Ti = 2.0L * vj * Tk - Tk_;
            Tk_ = Tk;
            Tk  = Ti;
            yCalc += m_coeffs[k] * Tk;
        }

        Ldbl err = yCalc - yOrig;
        if (fabsl(err) > errAbsMax) errAbsMax = fabsl(err);
        Ldbl errPct = (yOrig != 0.0L) ? err * 100.0L / yOrig : 0.0L;

        m_fittedPoints.append(QPointF((double)xj, (double)yCalc));

        outTxt += QString("[%1] X=%2 Y=%3 Ytch=%4 Err=%5 %6\n")
                     .arg(j)
                     .arg((double)xj, 0, 'g', 8)
                     .arg((double)yOrig, 0, 'g', 8)
                     .arg((double)yCalc, 0, 'g', 8)
                     .arg((double)err, 0, 'g', 6)
                     .arg((yOrig != 0.0L) ? QString("%1%").arg((double)errPct, 0, 'g', 4) : "---");
    }

    outTxt += QString("\nErreur Absolue Max = %1\n").arg((double)errAbsMax, 0, 'g', 8);
    m_errLabel->setText(QString("Erreur Absolue Max: %1").arg((double)errAbsMax, 0, 'g', 8));
    m_output->setTextColor(Qt::black);
    m_output->setPlainText(outTxt);

    // Evaluate fit on a dense grid for smooth curve
    m_fittedPoints.clear();
    int dense = qMax(200, m_np * 4);
    for (int i = 0; i <= dense; i++) {
        Ldbl x = m_xminVal + (m_xmaxVal - m_xminVal) * (Ldbl)i / dense;
        Ldbl v = 2.0L * (x - m_xminVal) / (m_xmaxVal - m_xminVal) - 1.0L;

        Ldbl y = m_coeffs[0] + m_coeffs[1] * v;
        Ldbl Tk_ = 1.0L, Tk = v;
        for (int k = 2; k < m_np; k++) {
            Ldbl Ti = 2.0L * v * Tk - Tk_;
            Tk_ = Tk;
            Tk  = Ti;
            y += m_coeffs[k] * Tk;
        }
        m_fittedPoints.append(QPointF((double)x, (double)y));
    }
}

Ldbl MainWindow::functionFT(int p, Ldbl t1)
{
    (void)p;
    // Example: f(x) = 4*sin(x)/(e^x + e^(-x))
    // Replace with user expression parser or hardcoded function
    Ldbl ex = expl(t1);
    return 4.0L * sinl(t1) / (ex + 1.0L / ex);
}

void MainWindow::updateChart()
{
    m_inputSeries->clear();
    m_fittedSeries->clear();

    for (const auto &pt : m_inputPoints)
        m_inputSeries->append(pt);

    for (const auto &pt : m_fittedPoints)
        m_fittedSeries->append(pt);

    autoRange();
}

void MainWindow::autoRange()
{
    if (m_inputPoints.isEmpty()) return;

    double xmin = 1e100, xmax = -1e100;
    double ymin = 1e100, ymax = -1e100;

    for (const auto &pt : m_inputPoints) {
        if (pt.x() < xmin) xmin = pt.x();
        if (pt.x() > xmax) xmax = pt.x();
        if (pt.y() < ymin) ymin = pt.y();
        if (pt.y() > ymax) ymax = pt.y();
    }
    for (const auto &pt : m_fittedPoints) {
        if (pt.x() < xmin) xmin = pt.x();
        if (pt.x() > xmax) xmax = pt.x();
        if (pt.y() < ymin) ymin = pt.y();
        if (pt.y() > ymax) ymax = pt.y();
    }

    double dx = (xmax - xmin) * 0.1;
    double dy = (ymax - ymin) * 0.1;

    if (dx == 0) dx = 1;
    if (dy == 0) dy = 1;

    auto axes = m_chart->axes();
    if (axes.size() >= 2) {
        axes[0]->setRange(xmin - dx, xmax + dx);
        axes[1]->setRange(ymin - dy, ymax + dy);
    }
}

void MainWindow::onZoomIn()
{
    m_chart->zoom(1.5);
}

void MainWindow::onZoomOut()
{
    m_chart->zoom(0.666);
}

void MainWindow::onResetView()
{
    autoRange();
}

void MainWindow::onExportText()
{
    QString path = QFileDialog::getSaveFileName(this, "Exporter les resultats", "TCHBYCHV_resultats.txt", "Textes (*.txt)");
    if (path.isEmpty()) return;
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&f);
    out << m_output->toPlainText();
    f.close();
}
