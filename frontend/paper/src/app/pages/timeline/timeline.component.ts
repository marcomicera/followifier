import {ChangeDetectorRef, Component, OnInit} from '@angular/core';

import {
    Chart,
    ChartDataSets,
    ChartOptions,
    ChartPoint,
    ChartTooltipCallback,
    ChartTooltipOptions,
    ChartType
} from "chart.js";
import {ApiService, Device} from '../../service/api/api.service';
import * as moment from 'moment'
import {FollowifierColors} from "../../model/colors";

@Component({
    selector: 'timeline-cmp',
    moduleId: module.id,
    templateUrl: 'timeline.component.html',
    styleUrls: ['timeline.component.css']
})

export class TimelineComponent implements OnInit {

    public canvas: any;
    public data: string;
    public ctx;
    public index: number;
    public min: number;
    public max: number;
    public macList: string[];
    private deviceMac: string;
    private positionDevice: Device[];
    public timelineChart;
    public xMin;
    public yMin;
    public xMax;
    public yMax;
    public scatterChartDataSet: ChartDataSets[] = [{
        label: 'Devices',
        pointRadius: 10,
        pointBackgroundColor: FollowifierColors.devicesColor,
        backgroundColor: FollowifierColors.devicesColor,
        hoverBackgroundColor: FollowifierColors.devicesColor,
        data: []
    }];

    public scatterCallback: ChartTooltipCallback = {
        title(item: Chart.ChartTooltipItem[], data: Chart.ChartData): string | string[] {
            let ts = [] as string[];
            item.forEach(i => {
                const unixtime = ((data.datasets[i.datasetIndex].data[i.index] as ChartPoint).t) as number;
                ts.push(moment.unix(unixtime).format(this.dateFormat));
            });
            return ts;
        }
    };
    public scatterToolTipOptions: ChartTooltipOptions = {
        enabled: true,
        callbacks: this.scatterCallback
    };
    public scatterChartType: ChartType = 'scatter';
    public scatterChartOptions: ChartOptions = {
        responsive: true,
        legend: {
            display: false
        },
        scales: {
            xAxes: [{
                ticks: {
                    min: this.xMin,
                    max: this.xMax
                }
            }],
            yAxes: [{
                ticks: {
                    stepSize: 100,
                    min: this.yMin,
                    max: this.yMax
                }
            }]
        },
        tooltips: this.scatterToolTipOptions,
    };
    private readonly dateFormat: string = 'DD/MM/YY HH:mm:ss';

    constructor(private apiService: ApiService, private changeDetectorRefs: ChangeDetectorRef) {
    }

    getTimeline(): void {
        this.apiService.getAllMacPosition(this.deviceMac).subscribe(positions => {
            console.log(`Device ${this.deviceMac} positions:`);
            console.dir(positions);
            console.log(positions.length);
            this.scatterChartDataSet[0].data = [];
            this.positionDevice = positions;
            this.max = this.positionDevice.length - 1;
            this.data = moment.unix(+this.positionDevice[0]._id).format(this.dateFormat);
            (this.scatterChartDataSet[0].data as ChartPoint[]).push({
                x: this.positionDevice[0].x,
                y: this.positionDevice[0].y,
                t: this.positionDevice[0]._id
            });
            this.updateTimelineWithIndex(0);
        });
    }

    ngOnInit() {

        // Room size
        this.apiService.getRoomCoordinate().subscribe(data => {
            this.yMax = 0;
            this.yMin = 600;
            this.xMin = 600;
            this.xMax = 0;

            console.dir(data);

            data.forEach(d => {
                if ( +d.y > this.yMax) { this.yMax = +d.y}
                if ( +d.y < this.yMin) { this.yMin = +d.y}
                if ( +d.x < this.xMin) { this.xMin = +d.x}
                if ( +d.x > this.xMax) { this.xMax = +d.x}
            });
            this.timelineChart.config.options.scales.yAxes[0].ticks.max = this.yMax;
            this.timelineChart.config.options.scales.yAxes[0].ticks.min = this.yMin;
            this.timelineChart.config.options.scales.xAxes[0].ticks.max = this.xMax;
            this.timelineChart.config.options.scales.xAxes[0].ticks.min = this.xMin;
            this.timelineChart.update();
        });

        this.min = 0;
        this.max = 0;
        // Retrieving all devices
        this.apiService.getAllMacDevices().subscribe(mac => {
            this.macList = [];
            mac.forEach(m => {
                this.macList.push(m._id);
            });
            this.deviceMac = this.macList[0];

            this.getTimeline();
        });


        this.canvas = document.getElementById("timelineChart");
        this.ctx = this.canvas.getContext("2d");
        this.timelineChart = new Chart(this.ctx, {
            type: this.scatterChartType,
            data: {
                datasets: this.scatterChartDataSet
            },
            options: this.scatterChartOptions
        });
    }

    updateTimelineWithIndex(sliderIndex) {
        this.scatterChartDataSet[0].data = [];
        this.data = moment.unix(+this.positionDevice[sliderIndex]._id).format(this.dateFormat);

        (this.scatterChartDataSet[0].data as ChartPoint[]).push({
            x: this.positionDevice[sliderIndex].x,
            y: this.positionDevice[sliderIndex].y,
            t: this.positionDevice[sliderIndex]._id
        });
        this.timelineChart.update();
    }

    updateTimeline(event) {
        this.updateTimelineWithIndex(event.value);
    }
}
