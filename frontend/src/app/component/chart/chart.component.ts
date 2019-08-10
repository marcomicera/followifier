import {Component, ElementRef, HostBinding, OnInit, ViewChild} from '@angular/core';
import {Chart} from 'chart.js';
import {log} from 'util';
import {DataService} from '../../service/data/data.service';
import {TestModel} from '../../model/testModel';

@Component({
  selector: 'app-chart',
  templateUrl: './chart.component.html',
  styleUrls: ['./chart.component.scss']
})
export class ChartComponent implements OnInit {

  @ViewChild('canvas', {static: true})
  canvas: ElementRef<HTMLCanvasElement>;

  @ViewChild('popup', {static: true})
  popup: ElementRef<HTMLDivElement>;

  private ctx: CanvasRenderingContext2D;
  private data: TestModel[];
  private imageWidth = 10;
  private imageHeight = 20;

  info: TestModel;
  open = undefined;

  constructor(private dataService: DataService) {
  }

  ngOnInit(): void {
    this.ctx = this.canvas.nativeElement.getContext('2d');
    // another method for later
    // this.canvas.nativeElement.height = window.innerHeight * 70 / 100;
    // this.canvas.nativeElement.width = window.innerWidth * 70 / 100;
    this.canvas.nativeElement.height = 600;
    this.canvas.nativeElement.width = 800;
    this.dataService.dailyForecast()
      .subscribe(
        data => {
          this.data = data;
        }, () => null, () => {
          this.data.forEach(d => this.draw(d.x, d.y));
        });
  }


  private draw(x, y): void {
    const image = new Image();
    image.onload = () => {
      // to center coordinates
      this.ctx.drawImage(image, x - (this.imageWidth / 2), y - (this.imageHeight / 2), this.imageWidth, this.imageHeight);
    };
    image.src = 'assets/img/phone.png';
  }

  public click(event): void {
    let done = false;
    this.data.forEach(value => {
      if (event.offsetX >= value.x - (this.imageWidth / 2) && event.offsetX <= value.x + (this.imageWidth / 2) &&
        event.offsetY >= value.y - (this.imageHeight / 2) && event.offsetY <= value.y + (this.imageHeight / 2) && !done) {
        this.popup.nativeElement.style.top = event.offsetY + this.canvas.nativeElement.offsetTop + 'px';
        this.popup.nativeElement.style.left = event.offsetX + this.canvas.nativeElement.offsetLeft + 'px';
        this.popup.nativeElement.style.visibility = 'visible';
        this.info = value;
        this.open = event;
        done = true;
      }
    });
  }

  public closePopup(event): void {
    if ( this.open !== undefined && this.open !== event) {
      this.popup.nativeElement.style.top = 0 + 'px';
      this.popup.nativeElement.style.left = 0 + 'px';
      this.popup.nativeElement.style.visibility = 'hidden';
      this.open = undefined;
      this.info = undefined;
    }
  }
}
