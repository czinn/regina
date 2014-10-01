
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#import "TextViewController.h"
#import "packet/ntext.h"

// TODO: Need to change insets to account for space taken by the keyboard.

@interface TextViewController () <UITextViewDelegate> {
    BOOL myEdit;
}
@property (weak, nonatomic) IBOutlet UITextView *detail;
@property (assign, nonatomic) regina::NText* packet;
@end

@implementation TextViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    _detail.delegate = self;

    [self reloadPacket];
}

- (void)reloadPacket {
    if (myEdit)
        return;

    _detail.text = [NSString stringWithUTF8String:self.packet->getText().c_str()];
}

- (void)endEditing
{
    // As a consequence, this calls textViewDidEndEditing:,
    // which is where the real work is done.
    [self.detail resignFirstResponder];
}

- (void)textViewDidEndEditing:(UITextView *)textView
{
    myEdit = YES;
    self.packet->setText([textView.text UTF8String]);
    myEdit = NO;
}

@end
