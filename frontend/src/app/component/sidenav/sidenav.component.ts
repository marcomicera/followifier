import {Component, OnInit} from '@angular/core';

interface ROUTE {
  route?: string;
  title?: string;
}

@Component({
  selector: 'app-sidenav',
  templateUrl: './sidenav.component.html',
  styleUrls: ['./sidenav.component.scss']
})
export class SidenavComponent implements OnInit {

  constructor() {
  }

  menuRoutes: ROUTE[] = [
    {
      route: '',
      title: 'Home',
    }, {
      route: 'stats',
      title: 'Stats',
    },{
      route: 'chart',
      title: 'Chart'
    }
  ];

  ngOnInit() {
  }

}
