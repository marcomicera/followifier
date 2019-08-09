import {Component, ElementRef, HostBinding, OnInit, ViewChild} from '@angular/core';
import {Chart} from 'chart.js';
import {log} from 'util';
import {DataService} from '../../data.service';
import {TestModel} from '../../model/testModel';

@Component({
  selector: 'app-chart',
  templateUrl: './chart.component.html',
  styleUrls: ['./chart.component.scss']
})
export class ChartComponent implements OnInit {

  @ViewChild('canvas', {static: true})
  canvas: ElementRef<HTMLCanvasElement>;

  private ctx: CanvasRenderingContext2D;
  private data: TestModel[];

  constructor(private dataService: DataService) {
  }

  ngOnInit(): void {
    this.ctx = this.canvas.nativeElement.getContext('2d');
    this.ctx.fillStyle = 'red';
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
      this.ctx.drawImage(image, x - 8, y - 10, 16, 20);
    }
    image.src = 'assets/img/phone.png';
  }

  public click(event): void {
    this.data.forEach(value => {
      if (event.offsetX >= value.x - 8 && event.offsetX <= value.x + 8 &&
        event.offsetY >= value.y - 8 && event.offsetY <= value.y + 8){
        alert('Clicked ' + value.name);
      }
    });
  }
}
