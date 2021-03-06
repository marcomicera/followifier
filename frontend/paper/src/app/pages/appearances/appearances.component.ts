import {ChangeDetectorRef, Component, OnInit} from '@angular/core';
import {ApiService, DeviceHistorical} from '../../service/api/api.service';
import * as moment from 'moment';
declare interface TableData {
    headerRow: string[];
    dataRows: string[][];

}

@Component({
    selector: 'table-cmp',
    moduleId: module.id,
    templateUrl: 'appearances.component.html',
    styleUrls: ['appearances.component.css']
})

export class AppearancesComponent implements OnInit {

    public macList: string[];
    public timeValue: number = 1;
    timeWindowValues = ['minutes', 'hours', 'days']
    public timeWindow: string = "hours";
    public devicesHistorical: DeviceHistorical[];
    private startDate: string;
    private  endDate: string;
    date: string;
    private readonly dateFormat: string = 'DD/MM/YY HH:mm:ss';
    public pag: string[] = [];
    public showTable: boolean;
    public showAppearances: boolean;
    public noDeviceFound: boolean;
    public mac: string;
    constructor(private apiService: ApiService, private changeDetectorRefs: ChangeDetectorRef) {
    }

    getAppearances(): void {
        let minutesInWindow = new Map([
            ['minutes', 1],
            ['hours', 60],
            ['days', 60 * 24]
        ]);

    }
  fillTable(): void {
    this.showTable = false;
    this.devicesHistorical = [];
    this.startDate = Date.parse(this.date[0]).toString().substring(0, 10);
    this.endDate = Date.parse(this.date[1]).toString().substring(0, 10);
    console.log(this.startDate);
    console.log(this.endDate);
    this.apiService.getDevicesHistorical(this.startDate, this.endDate).subscribe(devices => {
         if (devices.length > 0) {
           this.showTable = true;
           this.noDeviceFound = false;
           this.devicesHistorical = devices;
            this.changeDetectorRefs.detectChanges();
         } else {
           this.noDeviceFound = true;
           this.showAppearances = false;
           this.pag = [];


         }
       });
  }
  findInterval(mac): void {
    this.mac = mac;
    this.pag = [];
    this.showAppearances = true;
    this.apiService.getDevicesHistoricalIntervalls(this.startDate, this.endDate, mac).subscribe(devices => {
      devices.sort((one, two) => (one._id['timestamp'] > two._id['timestamp'] ? -1 : 1));
        devices.forEach(d => {
             this.pag.push(moment.unix(+(d._id['timestamp'])).format(this.dateFormat));
        })
      });
  }
    ngOnInit(): void {
        this.showTable = false;
        this.showAppearances = false;
        this.getAppearances();
    }
}
