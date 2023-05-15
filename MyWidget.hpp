#pragma once

#include <QWidget>
#include <QTimer>
#include <qwt/QwtPlot>
#include <qwt/QwtPlotCurve>

#include <mutex>
#include <iostream>

class MyWidget : public QwtPlot {
public:
    size_t samples_per_frame;
    size_t frames_to_draw;

    class BoundingRect : public QwtPlotCurve {
        size_t samples_per_frame;
        size_t frames_to_draw;
    public:
        BoundingRect(size_t frames_to_draw, size_t samples_per_frame)
            : QwtPlotCurve()
            , samples_per_frame(samples_per_frame)
            , frames_to_draw(frames_to_draw)
        {}

        QRectF boundingRect() const override {
            return QRectF(0.0, 0.0, frames_to_draw*samples_per_frame, 30.0);
        }
    };

    Q_OBJECT
    QwtPlotCurve *curve;
    QTimer* timer;

    void replot() override {
        QwtPlot::replot();
        replotDone();
    }
public:
    std::vector<double> samples;
    std::mutex mtx;

    MyWidget(size_t samples_per_frame, size_t frames_to_draw)
        : curve(new BoundingRect(frames_to_draw, samples_per_frame))
        , samples(samples_per_frame*frames_to_draw)
        , frames_to_draw(frames_to_draw)
        , samples_per_frame(samples_per_frame)
        , timer(new QTimer(this))
    {

        setTitle("Plot Demo");
        setCanvasBackground(Qt::black);

        curve->setTitle("Some Points");
        curve->setPen(Qt::white);

        // set the curve in the plot
        curve->attach(this);

        connect(this, &MyWidget::samplesUpdated, [this](){
            std::lock_guard lk(mtx);
            curve->setSamples(&samples.front(), samples.size());

        });

        timer->setSingleShot(true);
        connect(this, &MyWidget::replotDone, [this]{
            timer->start(2);
        });
        connect(timer, &QTimer::timeout, [this]{
            std::lock_guard lk(mtx);
            replot();
        });
    }

    void setSamples(std::vector<double> s) {
        static uint32_t counter = 0;

        if(std::lock_guard lk(mtx); true)
            std::move(s.begin(), std::next(s.begin()+samples_per_frame), std::next(samples.begin(), samples_per_frame*counter));

        ++counter %= frames_to_draw;
        if(counter == 0)
            samplesUpdated();
    }
signals:
    void samplesUpdated();
    void replotDone();
};
