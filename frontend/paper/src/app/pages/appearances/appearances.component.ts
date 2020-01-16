import {ChangeDetectorRef, Component, OnInit} from '@angular/core';
import {ApiService, DeviceHistorical} from '../../service/api/api.service';

declare interface TableData {
    headerRow: string[];
    dataRows: string[][];

}

@Component({
    selector: 'table-cmp',
    moduleId: module.id,
    templateUrl: 'appearances.component.html'
})

export class AppearancesComponent implements OnInit {

    public macList: string[];
    public timeValue: number = 1;
    timeWindowValues = ['minutes', 'hours', 'days']
    public timeWindow: string = "hours";
    public devicesHistorical: DeviceHistorical[];
    private startDate: string;
    private  endDate: string;

    constructor(private apiService: ApiService, private changeDetectorRefs: ChangeDetectorRef) {
    }

    getAppearances(): void {
        let minutesInWindow = new Map([
            ['minutes', 1],
            ['hours', 60],
            ['days', 60 * 24]
        ]);

    }
    startDateEvent(event): void {
      console.log(event['value']);
      console.log('------------------------');
      this.startDate = Date.parse(event['value']).toString().substring(0, 10);

      console.log(this.startDate );
    }
    endDateEvent(event): void {
    console.log(event['value']);
    console.log('------------------------');
    this.endDate = Date.parse(event['value']).toString().substring(0, 10);
    console.log(this.endDate);
  }
  fillTable(): void {
    this.apiService.getDevicesHistorical(this.startDate, this.endDate)
        .subscribe(devices => {
            console.dir(devices);
            this.devicesHistorical = devices;
            this.changeDetectorRefs.detectChanges();
        });
  }
    ngOnInit(): void {
        this.getAppearances();
    }
}
