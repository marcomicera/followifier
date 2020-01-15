import { Component, OnInit } from '@angular/core';
import {Chart, ChartDataSets, ChartPoint, ChartTooltipItem} from 'chart.js';
import {interval, Subscription} from "rxjs";
import {ActivatedRoute} from "@angular/router";
import {ApiService} from "../../service/api/api.service";


@Component({
    selector: 'dashboard-cmp',
    moduleId: module.id,
    templateUrl: 'dashboard.component.html'
})

export class DashboardComponent implements OnInit {

    public canvas: any;
    public ctx;
    public numberOfDevices;
    public radar;
    private updateSubscription: Subscription;
    public xMin;
    public yMin;
    public xMax;
    public yMax;



    constructor(private route: ActivatedRoute, private apiService: ApiService) {
    }

    // Number of boards
    totalNumberOfBoards: number;

    // Number of devices
    totalNumberOfDevices: number;
    private static readonly numberOfDevicesUpdatingFrequency = 2 * 1000;  // low value for testing purposes
    private static readonly numberOfDevicesUpdatingFrequencyLabel: string =
        ` Updated every ${DashboardComponent.numberOfDevicesUpdatingFrequency / 1000} seconds`;
    private static readonly maxNumberOfDevicesDataPoints = 10;

    // Radar
    private static readonly radarUpdatingFrequency = 2 * 1000; // low value for testing purposes
    private static readonly radarUpdatingFrequencyLabel: string =
        ` Updated every ${DashboardComponent.radarUpdatingFrequency / 1000} seconds`
    private radarToolTipItem: ChartTooltipItem[] = [];
    private radarDataSet: ChartDataSets[] = [{
        label: 'Boards',
        pointRadius: 10,
        pointBackgroundColor: '#51bcd9',
        backgroundColor: '#51bcd9',
        hoverBackgroundColor: '#51bcd9',
        data: [],
        hideInLegendAndTooltip: false,
    },
        {
            label: 'Devices',
            pointRadius: 10,
            pointBackgroundColor: '#fbc658',
            backgroundColor: '#fbc658',
            hoverBackgroundColor: '#fbc658',
            data: []
        }];

    ngOnInit() {

        // Room
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
            this.radar.config.options.scales.yAxes[0].ticks.max = this.yMax;
            this.radar.config.options.scales.yAxes[0].ticks.min = this.yMin;
            this.radar.config.options.scales.xAxes[0].ticks.max = this.xMax;
            this.radar.config.options.scales.xAxes[0].ticks.min = this.xMin;
            document.getElementById('roomDimensions').innerText = `${this.xMax}x${this.yMax}`;
            document.getElementById('squareMeters').innerText= `${this.xMax * this.yMax / 10000}`;
            this.radar.update();
        });

        // Number of devices updating frequency
        document.getElementById('totalNumberOfDevicesUpdatingFrequency')
            .insertAdjacentText('afterend', DashboardComponent.numberOfDevicesUpdatingFrequencyLabel);
        document.getElementById('numberOfDevicesUpdatingFrequency')
            .insertAdjacentText('afterend', DashboardComponent.numberOfDevicesUpdatingFrequencyLabel);

        // Number of devices
        this.canvas = document.getElementById("numberOfDevicess");
        this.ctx = this.canvas.getContext("2d");
        this.numberOfDevices = new Chart(this.ctx, {
            type: 'line',
            data: {
                labels: ["5 mins ago", "4 mins ago", "3 mins ago", "2 mins ago", "1 min ago"],
                datasets: [{
                    barPercentage: 1.6,
                    label: 'Devices',
                    borderColor: "#6bd098",
                    backgroundColor: "#6bd098",
                    pointRadius: 0,
                    pointHoverRadius: 0,
                    borderWidth: 3,
                    data: []
                }
                ]
            },
            options: {
                responsive: true,
                legend: {
                    display: false
                },
                tooltips: {
                    enabled: false
                },
                scales: {
                    yAxes: [{
                        ticks: {
                            min: 0,
                            stepSize: 1,
                            fontColor: "#9f9f9f",
                            beginAtZero: false,
                            maxTicksLimit: 5,
                            //padding: 20
                        },
                        gridLines: {
                            drawBorder: false,
                            zeroLineColor: "#ccc",
                            color: 'rgba(255,255,255,0.05)'
                        }
                    }],
                    xAxes: [{
                        gridLines: {
                            drawBorder: false,
                            color: 'rgba(255,255,255,0.1)',
                            zeroLineColor: "transparent",
                            display: false,
                        },
                        ticks: {
                            padding: 20,
                            fontColor: "#9f9f9f"
                        }
                    }]
                },
            }
        });
        this.updateSubscription = interval(DashboardComponent.numberOfDevicesUpdatingFrequency).subscribe((val) => {
            console.log('Updating devices number line chart');
            this.apiService.getDevicesNumber().subscribe(data => {
                    this.totalNumberOfDevices = +data;
                    this.numberOfDevices.data.datasets[0].data.push(this.totalNumberOfDevices);
                    // this.lineChartLabels.push(String(new Date().getMinutes()));
                    if (this.numberOfDevices.data.datasets[0].data.length > DashboardComponent.maxNumberOfDevicesDataPoints) {
                        this.numberOfDevices.data.datasets[0].data.shift();
                        // this.lineChartLabels.shift();
                    }
                    this.numberOfDevices.chart.update();
                    document.getElementById("totalNumberOfDevices").innerText = String(this.totalNumberOfDevices);
                }
            );
        });


        // Radar
        this.canvas = document.getElementById("radar");
        this.ctx = this.canvas.getContext("2d");
        document.getElementById('radarUpdatingFrequency')
            .insertAdjacentText('afterend', DashboardComponent.radarUpdatingFrequencyLabel);
        this.radar = new Chart(this.ctx, {
            type: 'scatter',
            // hover: false,
            data: {
                labels: [],
                datasets: this.radarDataSet
            },
            options: {
                legend: {
                    display: false,
                    position: 'top'
                },
                tooltips: {
                     callbacks: {
                        title(item: Chart.ChartTooltipItem[], data: Chart.ChartData): string | string[] {
                            let ts = [] as string[];
                            item.forEach(i => {
                                ts.push((data.datasets[item[0].datasetIndex].data[item[0].index] as ChartPoint).t as string);
                            });
                            return ts;
                        }
                    }
                },
                scales: {
                    xAxes: [{
                        ticks: {
                            min: this.xMin,
                            max: this.xMax  // FIXME room dimension
                        }
                    }],
                    yAxes: [{
                        ticks: {
                            stepSize: 100,
                            min: this.yMin,
                            max: this.yMax // FIXME room dimension
                        }
                    }]
                },
            }
        });

        this.updateSubscription = interval(DashboardComponent.radarUpdatingFrequency).subscribe((val) => {
            console.log('Updating radar');
            this.radarToolTipItem = [];
            this.apiService.getBoards().subscribe(data => {
                this.radarDataSet[0].data = [];

                data.forEach(d => {
                   console.log('board: ' + d.mac);

                    (this.radarDataSet[0].data as ChartPoint[]).push({x: d.x, y: d.y, t: d.mac});
                    (this.radarToolTipItem.push({
                        label: d.mac,
                        datasetIndex: 0,
                        index: this.radarDataSet[0].data.length
                    }));

                });

                document.getElementById("totalNumberOfBoards").innerText = String(data.length);
            });
            this.apiService.getDevices().subscribe(data => {
                this.radarDataSet[1].data = [];
                data.forEach(d => {
                    console.log('device: ' + d._id);
                    (this.radarDataSet[1].data as ChartPoint[]).push({x: d.x, y: d.y, t: d._id});
                    (this.radarToolTipItem.push({
                        label: d._id,
                        datasetIndex: 1,
                        index: this.radarDataSet[1].data.length
                    }));
                });
            });
            this.radar.update();
        });
    }
}
