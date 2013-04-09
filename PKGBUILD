pkgname=mpd-notify-git
pkgbase=mpd-notify
pkgver=20120921
pkgrel=1
pkgdesc="Notifies you about MPD"
arch=('i686' 'x86_64')
url="https://github.com/Unia/mpd-notify"
depends=('libnotify' 'libmpdclient')
license=('GPL')

_gitroot="https://github.com/Unia/mpd-notify.git"
_gitname="${pkgbase}"

pkgver() {
    cd "$srcdir/$_gitname"
    git log -1 --format="%cd" --date=short | sed 's|-|.|g'
}

package() {
  cd ${srcdir}/
    msg "Connecting to GIT server...."
    
    if [ -d ${_gitname} ] ; then
        cd ${_gitname} && git pull origin
        msg "The local files are updated."
    else
        git clone ${_gitroot} ${_gitname}
    fi
    msg "GIT checkout done or server timeout"

  cd ${srcdir}/${pkgbase}

  make
}

build() {
  cd ${srcdir}/${pkgbase}

  make DESTDIR="${pkgdir}" install
}
